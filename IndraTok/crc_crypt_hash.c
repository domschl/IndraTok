#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "crc_crypt_hash.h"

// ======== CRC-CCITT ================================================
// Source: Dr.Dobb's, Bob Felice, June 17, 2007,
// http://www.drdobbs.com/implementing-the-ccitt-cyclical-redundan/199904926
// The CCITT CRC 16 polynomial is X^16 + X^12 + X^5 + 1.
// In binary, this is the bit pattern 1 0001 0000 0010 0001, and in hex it is 0x11021.
// A 17 bit register is simulated by testing the MSB before shifting the data, which
// affords us the luxury of specifiy the polynomial as a 16 bit value, 0x1021.
// Due to the way in which we process the CRC, the bits of the polynomial are stored
// in reverse order. This makes the polynomial 0x8408.

#define CRC16_CCITT_POLY (0x8408)

uint16_t crc16_ccitt(const uint8_t *data_p, unsigned long length) {
  uint8_t i;
  uint16_t data;
  uint16_t crc;

  crc = 0xffff;

  if (length == 0) {
    return (~crc);
  }
  do {
    for (i = 0, data = (unsigned int)0xff & *data_p++; i < 8; i++, data >>= 1) {
      if ((crc & 0x0001) ^ (data & 0x0001)) {
        crc = (crc >> 1) ^ CRC16_CCITT_POLY;
      } else {
        crc >>= 1;
      }
    }
  } while (--length);

  crc = ~crc;
  data = crc;
  crc = (crc << 8) | (data >> 8 & 0xFF);
  return (crc);
}

// ======== XX-TEA ===================================================
// Source: http://en.wikipedia.org/wiki/XXTEA
// According to Needham and Wheeler:
// BTEA will encode or decode n words as a single block where n > 1
// v is the n word data vector
// k is the 4 word key
// assumes 32 bit 'long' and same endian coding and decoding
// Beware! in situ!


#define DELTA (0x9e3779b9)
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void xxtea_encrypt(uint32_t *v, int n, uint32_t const key[4]) {
  uint32_t y, z, sum;
  uint16_t p, rounds, e;
    
  if (n<1) return;
    
  rounds = 6 + 52/n;
  sum = 0;
  z = v[n-1];
  do {
    sum += DELTA;
    e = (sum >> 2) & 3;
    for (p=0; p<n-1; p++) {
      y = v[p+1];
      z = v[p] += MX;
    }
    y = v[0];
    z = v[n-1] += MX;
  } while (--rounds);
}

void xxtea_decrypt(uint32_t *v, int n, uint32_t const key[4]) {
  uint32_t y, z, sum;
  uint16_t p, rounds, e;
    
  if (n<1) return;
    
  rounds = 6 + 52/n;
  sum = rounds*DELTA;
  y = v[0];
  do {
    e = (sum >> 2) & 3;
    for (p=n-1; p>0; p--) {
      z = v[p-1];
      y = v[p] -= MX;
    }
    z = v[n-1];
    y = v[0] -= MX;
    sum -= DELTA;
  } while (--rounds);
}

// ======== uHash-MAP ================================================

uHash* uHashCreate(unsigned long size) {
  uHash *puh = (uHash *)malloc(sizeof(uHash));
  memset((void *)puh, 0, sizeof(uHash));
  if (size) {
    puh->pHE = (uHashEntry *)malloc(sizeof(uHashEntry)*size);
    if (puh->pHE) {
      puh->size = size;
      puh->entries = 0;
    } else {
      free(puh);
      return NULL;
    }
  }
  return puh;
}

void uHashDelete(uHash *puHash) {
  if (puHash == NULL) return;
  if (puHash->size > 0 && puHash->pHE) {
    for (unsigned long l; l<puHash->size; l++) {
      if (puHash->pHE[l].keySize && puHash->pHE[l].pKey) free(puHash->pHE[l].pKey);
      if (puHash->pHE[l].valueSize && puHash->pHE[l].pValue) free(puHash->pHE[l].pValue);
    }
  }
  free(puHash);
}

uHash* uHashGrow(uHash *puHash, unsigned long newSize) {
  if (!puHash) return NULL;
  if (newSize < puHash->size) return NULL;
  uHash *puNew = uHashCreate(newSize);
  if (!puNew) return NULL;
  for (unsigned long l; l<puHash->size; l++) {
    puNew->pHE[l] = puHash->pHE[l];
  }
  free(puHash);
  return puNew;
}

long uHashIndex(uHash *puHash, const void *pKey, unsigned long keySize) {
  if (puHash->entries == 0) return -1;
  unsigned long hash = (unsigned long)crc16_ccitt((const uint8_t *)pKey, keySize);
  unsigned long a=0, d=0;
  unsigned long b=puHash->entries-1;
  while (b-a >= 1) {
    d = (a+b)/2;  // a + (b-a)/2;  
    if (puHash->pHE[a].hash==hash) break;
    if (puHash->pHE[b].hash==hash) {
      a=b;
      break;
    }
    if (puHash->pHE[d].hash==hash) {
      a=d;
      break;
    }
    if (puHash->pHE[d].hash < hash) {
      if (b==d) {
        a=d;
        break;
      }
      b = d;
    } else {
      if (a==d) break;
      a = d;
    }
  }
  while (puHash->pHE[a].hash == hash) {
    if (puHash->pHE[a].keySize == keySize && !memcmp(puHash->pHE[a].pKey, pKey, keySize)) return a;
    ++a;
    if (a>=puHash->entries) break;
  }  
  return -a - 1;
}

void uHashInsert(uHash *puHash, const void *pKey, unsigned long keySize, const void *pValue, unsigned long valueSize) {
  if (!puHash || !pKey || !keySize || !pValue || !valueSize) return;
  if (puHash->size == puHash->entries) return;
  long idx, ins_idx;
  idx = uHashIndex(puHash, pKey, keySize);
  if (idx>=0) {
    // Existing key
    ins_idx = idx;
    free(puHash->pHE[ins_idx].pKey);
    free(puHash->pHE[ins_idx].pValue);
  } else {
    // New key
    ins_idx = -idx -1; 
    for (unsigned long li=puHash->entries; li>ins_idx; li--) {
      puHash->pHE[li] = puHash->pHE[li-1];
    }
    ++(puHash->entries);
  }
  puHash->pHE[ins_idx].hash = crc16_ccitt((const uint8_t *)pKey, keySize);
  puHash->pHE[ins_idx].pKey = malloc(keySize);
  memcpy(puHash->pHE[ins_idx].pKey, pKey, keySize);
  puHash->pHE[ins_idx].keySize = keySize;
  puHash->pHE[ins_idx].pValue = malloc(valueSize);
  memcpy(puHash->pHE[ins_idx].pValue, pValue, valueSize);
  puHash->pHE[ins_idx].valueSize = valueSize;
}

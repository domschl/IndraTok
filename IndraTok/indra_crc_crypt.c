#include <stdint.h>

#include "indra_crc_crypt.h"

// ======== CRC-CCITT ================================================
// Source: Dr.Dobb's, Bob Felice, June 17, 2007,
// http://www.drdobbs.com/implementing-the-ccitt-cyclical-redundan/199904926
// The CCITT CRC 16 polynomial is X^16 + X^12 + X^5 + 1.
// In binary, this is the bit pattern 1 0001 0000 0010 0001, and in hex ia is 0x11021.
// A 17 bit register is simulated by testing the MSB before shifting the data, which
// affords us the luxury of specifiy the polynomial as a 16 bit value, 0x1021.
// Due to the way in which we process the CRC, the bits of the polynomial are stored
// in reverse order. This makes the polynomial 0x8408.

#define IA_CRC16_CCITT_POLY (0x8408)

uint16_t iaCrc16Ccitt(const uint8_t *data_p, unsigned long length) {
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
        crc = (crc >> 1) ^ IA_CRC16_CCITT_POLY;
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

#define IA_SIMPLE_CONST 0x47218408
#define IA_SIMPLE_CONST2 0x4329567584745642
#define IA_SIMPLE_MULT 31

unsigned long iaSimpleHash(unsigned char *buf, unsigned long length) {
  unsigned long hash = 0xffffffffffffffff;
  unsigned long data;
  for (unsigned long i=0; i<length; i++) {
    data = 0;
    data |= (unsigned long)buf[i];
    if (hash & 1) data |= (unsigned long)buf[i] << 8;
    if (hash & 2) data |= (unsigned long)buf[i] << 16;
    if (hash & 4) data |= (unsigned long)buf[i] << 32;
    if (hash & 8) data |= (unsigned long)buf[i] << 40;
    if (hash & 16) data |= (unsigned long)buf[i] << 48;
    for (int j=0; j<4; j++) hash = ((hash ^ IA_SIMPLE_CONST2) * IA_SIMPLE_MULT)  + data;
  }
  return hash;
}


// ======== XX-TEA ===================================================
// Source: http://en.wikipedia.org/wiki/XXTEA
// According to Needham and Wheeler:
// BTEA will encode or decode n words as a single block where n > 1
// v is the n word data vector
// k is the 4 word key
// assumes 32 bit 'long' and same endian coding and decoding
// Beware! in situ!


#define IA_XXTEA_DELTA (0x9e3779b9)
#define IA_XXTEA_MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void iaXxteaEncrypt(uint32_t *v, int n, uint32_t const key[4]) {
  uint32_t y, z, sum;
  uint16_t p, rounds, e;
    
  if (n<1) return;
    
  rounds = 6 + 52/n;
  sum = 0;
  z = v[n-1];
  do {
    sum += IA_XXTEA_DELTA;
    e = (sum >> 2) & 3;
    for (p=0; p<n-1; p++) {
      y = v[p+1];
      z = v[p] += IA_XXTEA_MX;
    }
    y = v[0];
    z = v[n-1] += IA_XXTEA_MX;
  } while (--rounds);
}

void iaXxteaDecrypt(uint32_t *v, int n, uint32_t const key[4]) {
  uint32_t y, z, sum;
  uint16_t p, rounds, e;
    
  if (n<1) return;
    
  rounds = 6 + 52/n;
  sum = rounds*IA_XXTEA_DELTA;
  y = v[0];
  do {
    e = (sum >> 2) & 3;
    for (p=n-1; p>0; p--) {
      z = v[p-1];
      y = v[p] -= IA_XXTEA_MX;
    }
    z = v[n-1];
    y = v[0] -= IA_XXTEA_MX;
    sum -= IA_XXTEA_DELTA;
  } while (--rounds);
}

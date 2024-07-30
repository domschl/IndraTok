#pragma once

#include <stdlib.h>

uint16_t crc16_ccitt(const uint8_t *data_p, unsigned long length);

void xxtea_encrypt(uint32_t *v, int n, uint32_t const key[]);
void xxtea_decrypt(uint32_t *v, int n, uint32_t const key[]);

typedef struct _u_hash_entry {
  unsigned long keySize;
  void *pKey;
  unsigned long valueSize;
  void *pValue;
  unsigned long hash;
} uHashEntry;

typedef struct _u_hash {
  unsigned long size;
  unsigned long entries;
  uHashEntry *pHE;
} uHash;

uHash* uHashCreate(unsigned long size);
void uHashDelete(uHash *puHash);
uHash* uHashGrow(uHash *puHash, unsigned long newSize);

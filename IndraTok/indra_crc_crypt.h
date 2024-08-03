#pragma once

#include <stdint.h>

uint16_t iaCrc16Ccitt(const uint8_t *data_p, unsigned long length);
unsigned long iaSimpleHash(unsigned char *buf, unsigned long length);

void iaXxteaEncrypt(uint32_t *v, int n, uint32_t const key[]);
void iaXxteaDecrypt(uint32_t *v, int n, uint32_t const key[]);

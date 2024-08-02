#pragma once

#include <stdint.h>

uint16_t itCrc16Ccitt(const uint8_t *data_p, unsigned long length);

void itXxteaEncrypt(uint32_t *v, int n, uint32_t const key[]);
void itXxteaDecrypt(uint32_t *v, int n, uint32_t const key[]);

#pragma once

#include <stdbool.h>
#include "indra_atom.h"

bool iaStringContains(IA_T_ATOM *source, IA_T_ATOM *token);
long iaStringIndexFind(IA_T_ATOM *source, IA_T_ATOM *token, size_t offset);
size_t iaStringCountToken(IA_T_ATOM *source, IA_T_ATOM *token);
size_t iaCharUtf8Length(unsigned char c);
bool iaStringValidateUtf8(IA_T_ATOM *source);
size_t iaStringUtf8Length(IA_T_ATOM *source);
bool iaStringPartUtf8(IA_T_ATOM *source, IA_T_ATOM *pdest, size_t start, size_t len);

void iaStringDisplayHex(IA_T_ATOM *source);

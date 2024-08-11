#pragma once

#include <stdbool.h>
#include "indra_atom.h"

bool iaStringContains(IA_T_ATOM *source, IA_T_ATOM *token);
long iaStringFind(IA_T_ATOM *source, IA_T_ATOM *token, size_t offset);
size_t iaStringCountToken(IA_T_ATOM *source, IA_T_ATOM *token);
size_t iaCharUtf8Length(unsigned char c);
bool iaStringUtf8Validate(IA_T_ATOM *source);
size_t iaStringUtf8Length(IA_T_ATOM *source);
bool iaStringUtf8Part(IA_T_ATOM *source, IA_T_ATOM *pdest, size_t start, size_t len);

void iaStringDisplayHex(IA_T_ATOM *source);
bool iaStringUtf8Split(IA_T_ATOM *source, IA_T_ATOM *token, IA_T_ATOM *parts);
void debugAtom(IA_T_ATOM *pAtom);

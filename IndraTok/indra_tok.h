#pragma once

#include <stdbool.h>
#include "indra_atom.h"

void stringAppend(IA_T_ATOM *root, IA_T_ATOM *appendix);
void stringPartBytes(IA_T_ATOM *source, IA_T_ATOM **ppPart, unsigned int start, unsigned long len);
void stringStartBytes(IA_T_ATOM *source, IA_T_ATOM **ppStart, unsigned long len);
void stringEndBytes(IA_T_ATOM *source, IA_T_ATOM **ppEnd, unsigned long len);
bool stringContainsBytes(IA_T_ATOM *source, IA_T_ATOM *token);
long stringFindBytes(IA_T_ATOM *source, IA_T_ATOM *token, unsigned long offset);
long stringFindCountBytes(IA_T_ATOM *source, IA_T_ATOM *token);
unsigned int utf8CharLen(unsigned char ctok);
bool stringValidateUtf8(IA_T_ATOM *source);
/*void stringAppendUtf8(IA_T_ATOM *root, IA_T_ATOM *appendix);
unsigned int stringLenUtf8(IA_T_ATOM *source);
int stringFindUtf8(IA_T_ATOM *source, IA_T_ATOM *token);
long stringFindCountUtf8(IA_T_ATOM *source, IA_T_ATOM *token);
IA_T_ATOM* stringPartUtf8(IA_T_ATOM *source, unsigned int start, unsigned int len);
void stringDisplayHex(IA_T_ATOM *source);
IA_T_ATOM* stringSplitUtf8(IA_T_ATOM *source, IA_T_ATOM *token);
*/

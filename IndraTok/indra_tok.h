#pragma once

#include <stdbool.h>
#include "indra_atom.h"

void stringAppend(IndraAtom *root, const IndraAtom *appendix);
void stringPartBytes(const IndraAtom *source, IndraAtom **ppPart, unsigned int start, unsigned long len);
void stringStartBytes(const IndraAtom *source, IndraAtom **ppStart, unsigned long len);
void stringEndBytes(const IndraAtom *source, IndraAtom **ppEnd, unsigned long len);
bool stringContainsBytes(const IndraAtom *source, IndraAtom *token);
long stringFindBytes(const IndraAtom *source, const IndraAtom *token, unsigned long offset);
long stringFindCountBytes(const IndraAtom *source, const IndraAtom *token);
unsigned int utf8CharLen(unsigned char ctok);
bool stringValidateUtf8(const IndraAtom *source);
unsigned int stringLenUtf8(const IndraAtom *source);
int stringFindUtf8(const IndraAtom *source, const IndraAtom *token);
long stringFindCountUtf8(const IndraAtom *source, const IndraAtom *token);
IndraAtom* stringPartUtf8(const IndraAtom *source, unsigned int start, unsigned int len);
void stringDisplayHex(const IndraAtom *source);
IndraAtom* stringSplitUtf8(const IndraAtom *source, const IndraAtom *token);

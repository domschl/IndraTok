#pragma once

#include <stdbool.h>
#include "indra_ent.h"

void stringAppend(IndraEnt *root, const IndraEnt *appendix);
void stringPartBytes(const IndraEnt *source, IndraEnt **ppPart, unsigned int start, unsigned long len);
void stringStartBytes(const IndraEnt *source, IndraEnt **ppStart, unsigned long len);
void stringEndBytes(const IndraEnt *source, IndraEnt **ppEnd, unsigned long len);
bool stringContainsBytes(const IndraEnt *source, IndraEnt *token);
long stringFindBytes(const IndraEnt *source, const IndraEnt *token, unsigned long offset);
long stringFindCountBytes(const IndraEnt *source, const IndraEnt *token);
unsigned int utf8CharLen(unsigned char ctok);
bool stringValidateUtf8(const IndraEnt *source);
unsigned int stringLenUtf8(const IndraEnt *source);
int stringFindUtf8(const IndraEnt *source, const IndraEnt *token);
long stringFindCountUtf8(const IndraEnt *source, const IndraEnt *token);
void stringPartUtf8(const IndraEnt *source, IndraEnt **ppPart, unsigned int start, unsigned int len);
void stringDisplayHex(const IndraEnt *source);
IndraEntArray* stringSplitUtf8(const IndraEnt *source, const IndraEnt *token);

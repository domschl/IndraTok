#pragma once

#include <stdbool.h>

typedef enum _indra_types {IT_NIL=0, IT_CHAR, IT_BYTE, IT_BYTES, IT_STRING, IT_INT, IT_UINT, IT_LONG, IT_ULONG, IT_FLOAT, IT_DOUBLE, IT_INFNUM} IndraTypes;

typedef struct _indra_ent {
  void *buf;
  IndraTypes type;
  unsigned long len;
} IndraEnt;

typedef struct _indra_ent_array {
  IndraEnt *ieArray;
  IndraTypes type;
  unsigned long capacity;
  unsigned long count;
} IndraEntArray;

void itDelete(IndraEnt *pie);
IndraEnt *itCreateChar(char c);
IndraEnt *itCreateByte(unsigned char byte);
IndraEnt *itCreateBytes(const unsigned char bytes[], unsigned long len);
IndraEnt *itCreateString(const char str[]);
IndraEnt *itCreateStringByLength(unsigned long strLength);
IndraEnt *itCreateInt(int i);
IndraEnt *itCreateUInt(unsigned int ui);
IndraEnt *itCreateLong(long l);
IndraEnt *itCreateULong(unsigned long ul);
IndraEnt *itCreateFloat(float f);
IndraEnt *itCreateDouble(double df);

IndraEntArray *itCreateArray(IndraTypes type, unsigned long capacity);
void itDeleteArray(IndraEntArray *piea);
IndraEntArray *itResizeArray(IndraEntArray **, unsigned long capacity);
bool itaSet(IndraEntArray *piea, unsigned long index, IndraEnt *pie);
IndraEnt *itaGet(const IndraEntArray *piea, unsigned long index);
bool itaSetGrow(IndraEntArray **piea, unsigned long index, IndraEnt *pie);
bool itaDelete(IndraEntArray *piea, unsigned long index);
bool itaInsert(IndraEntArray *piea, unsigned long index, IndraEnt *pie);

void itPrint(IndraEnt *pie);
void itPrintLn(IndraEnt *pie);

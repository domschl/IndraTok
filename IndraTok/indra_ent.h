#pragma once

typedef enum _indra_types {IT_NIL=0, IT_CHAR, IT_BYTE, IT_BYTES, IT_STRING, IT_INT, IT_UINT, IT_LONG, IT_ULONG, IT_FLOAT, IT_DOUBLE, IT_INFNUM} IndraTypes;

typedef struct _indra_ent {
  void *buf;
  IndraTypes type;
  unsigned long len;
} IndraEnt;

typedef struct _indra_ent_array {
  void *buf;
  IndraTypes type;
  unsigned long elementLen;
  unsigned long count;
} IndraEntArray;

void itDelete(IndraEnt *pie);
IndraEnt *itCreateChar(char c);
IndraEnt *itCreateByte(unsigned char byte);
IndraEnt *itCreateBytes(unsigned char bytes[], unsigned long len);
IndraEnt *itCreateString(char str[]);
IndraEnt *itCreateInt(int i);
IndraEnt *itCreateUInt(unsigned int ui);
IndraEnt *itCreateLong(long l);
IndraEnt *itCreateULong(unsigned long ul);
IndraEnt *itCreateFloat(float f);
IndraEnt *itCreateDouble(double df);


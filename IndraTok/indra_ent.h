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

typedef enum _indra_hash_type {IT_HASH_CRC16, IT_HASH_SIMPLE} IndraHashTypes;
typedef struct _indra_ent_map {
  IndraHashTypes hashType;
  IndraEntArray *pHash;
  IndraEntArray *pKeys;
  IndraEntArray *pValues;
} IndraEntMap;

void itDelete(IndraEnt *pie);
IndraEnt *itCreateChar(char c);
IndraEnt *itCreateByte(unsigned char byte);
IndraEnt *itCreateBytes(const unsigned char bytes[], unsigned long len);
IndraEnt *itCreateString(const char str[]);
IndraEnt *itCreateStringByLength(unsigned long strLength);
IndraEnt *itCreateStringFromSlice(const IndraEnt *bufByteString, unsigned long bufSliceStart, unsigned long bufSliceLength);
IndraEnt *itCreateInt(int i);
IndraEnt *itCreateUInt(unsigned int ui);
IndraEnt *itCreateLong(long l);
IndraEnt *itCreateULong(unsigned long ul);
IndraEnt *itCreateFloat(float f);
IndraEnt *itCreateDouble(double df);

IndraEntArray *itArrayCreate(IndraTypes type, unsigned long capacity);
void itArrayDelete(IndraEntArray *piea);
IndraEntArray *itArrayResize(IndraEntArray **, unsigned long capacity);
bool itArraySet(IndraEntArray *piea, unsigned long index, IndraEnt *pie);
IndraEnt *itArrayGet(const IndraEntArray *piea, unsigned long index);
bool itArraySetGrow(IndraEntArray **piea, unsigned long index, IndraEnt *pie);
bool itArrayAppend(IndraEntArray **piea, IndraEnt *pie);
bool itArrayRemove(IndraEntArray *piea, unsigned long index);
bool itArrayInsert(IndraEntArray **piea, unsigned long index, IndraEnt *pie);

void itPrint(const IndraEnt *pie);
void itPrintLn(const IndraEnt *pie);
void itArrayPrint(const IndraEntArray *piea);
void itArrayPrintLn(const IndraEntArray *piea);

IndraEntMap *itMapCreateHash(IndraTypes keyType, IndraTypes valueType, IndraHashTypes);
IndraEntMap *itMapCreate(IndraTypes keyType, IndraTypes valueType);
void itMapDelete(IndraEntMap *piem);
long _itMapHashIndexGet(IndraEntMap *piem, IndraEnt *pKey, unsigned long *pHash);
long _itMapIndexGet(IndraEntMap *piem, IndraEnt *pKey, unsigned long *pHash);
bool itMapSet(IndraEntMap *piem, IndraEnt *pKey, IndraEnt *pValue);
IndraEnt* itMapGet(IndraEntMap *piem, IndraEnt *pKey);
bool itMapExists(IndraEntMap *piem, IndraEnt *pKey);
bool itMapRemove(IndraEntMap *piem, IndraEnt *pKey);
void itMapPrint(IndraEntMap *piem);

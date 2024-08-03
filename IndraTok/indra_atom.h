#pragma once

#include <stdbool.h>

typedef enum _indra_types {IA_NIL=0, IA_CHAR, IA_BYTE, IA_INT, IA_UINT, IA_LONG, IA_ULONG,
                           IA_FLOAT, IA_DOUBLE, IA_INFNUM, IA_ATOM, IA_MAP, IA_LIST} IndraTypes;
/*
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
*/
typedef struct _indra_atom {
  void *buf;
  IndraTypes type;
  unsigned long recsize;
  unsigned long capacity;
  unsigned long count;
} IndraAtom;

typedef struct _indra_atom_map {
} IndraAtomMap;

typedef struct _indra_atom_list {
} IndraAtomList;

const unsigned long indraTypesRecsize[] = {0, sizeof(char), sizeof(unsigned char),
                                              sizeof(int), sizeof(unsigned int), sizeof(long), sizeof(unsigned long),
                                              sizeof(float), sizeof(double), sizeof(unsigned char),
                                              sizeof(IndraAtom), sizeof(IndraAtomMap), sizeof(IndraAtomList)};

typedef enum _indra_hash_type {IA_HASH_CRC16, IA_HASH_SIMPLE} IndraHashTypes;

typedef struct _indra_old_ent_map {
  IndraHashTypes hashType;
  IndraAtom *pHash;
  IndraAtom *pKeys;
  IndraAtom *pValues;
} IndraOldEntMap;

void iaDelete(IndraAtom *pia);
IndraAtom *iaCreateChar(char c);
IndraAtom *iaCreateByte(unsigned char byte);
IndraAtom *iaCreateBytes(const unsigned char bytes[], unsigned long len);
IndraAtom *iaCreateString(const char str[]);
IndraAtom *iaCreateStringFromSlice(const IndraAtom *bufByteString, unsigned long bufSliceStart, unsigned long bufSliceLength);
IndraAtom *iaCreateInt(int i);
IndraAtom *iaCreateUInt(unsigned int ui);
IndraAtom *iaCreateLong(long l);
IndraAtom *iaCreateULong(unsigned long ul);
IndraAtom *iaCreateFloat(float f);
IndraAtom *iaCreateDouble(double df);
IndraAtom *iaCreate(IndraTypes type, void *buf, unsigned long count, unsigned long capacity);

IndraAtom *iaResize(IndraAtom **ppia, unsigned long capacity);
void *iaArrayGet(const IndraAtom *pia, unsigned long index);
bool iaArraySet(IndraAtom *pia, unsigned long index, const void *buf);
bool iaArraySetGrow(IndraAtom **pia, unsigned long index, const void *buf);
bool iaArrayAppend(IndraAtom **ppia, const void *buf);
bool iaArrayRemove(IndraAtom *pia, unsigned long index);
bool iaArrayInsert(IndraAtom **ppia, unsigned long index, const void *buf);

void iaPrint(const IndraAtom *pia);
void iaPrintLn(const IndraAtom *pia);

/*
IndraOldEntMap *iaOldMapCreateHash(IndraTypes keyType, IndraTypes valueType, IndraHashTypes);
IndraOldEntMap *iaOldMapCreate(IndraTypes keyType, IndraTypes valueType);
void iaOldMapDelete(IndraOldEntMap *piam);
long _itOldMapHashIndexGet(IndraOldEntMap *piam, IndraAtom *pKey, unsigned long *pHash);
long _itOldMapIndexGet(IndraOldEntMap *piam, IndraAtom *pKey, unsigned long *pHash);
bool iaOldMapSet(IndraOldEntMap *piam, IndraAtom *pKey, IndraAtom *pValue);
IndraAtom* iaOldMapGet(IndraOldEntMap *piam, IndraAtom *pKey);
bool iaOldMapExists(IndraOldEntMap *piam, IndraAtom *pKey);
bool iaOldMapRemove(IndraOldEntMap *piam, IndraAtom *pKey);
void iaOldMapPrint(IndraOldEntMap *piam);
*/

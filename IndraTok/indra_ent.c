#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_ent.h"
#include "indra_crc_crypt.h"


void itDelete(IndraEnt *pie) {
  if (pie == NULL) return;
  if (pie->type != IT_NIL && pie->buf != NULL) {
    free(pie->buf);
  }
  free(pie);
}

IndraEnt *itCreateChar(char c) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(char));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(char *)pie->buf=c;
  pie->type = IT_CHAR;
  pie->len = sizeof(char);
  return pie;
}

IndraEnt *itCreateByte(unsigned char byte) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned char));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned char *)pie->buf=byte;
  pie->type = IT_BYTE;
  pie->len = sizeof(unsigned char);
  return pie;
}

IndraEnt *itCreateBytes(const unsigned char bytes[], unsigned long len) {
  if (bytes == NULL) return NULL;
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned char) * len);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memcpy(pie->buf, bytes, sizeof(unsigned char) * len);
  pie->type = IT_BYTES;
  pie->len = sizeof(unsigned char) * len;
  return pie;
}

IndraEnt *itCreateString(const char str[]) {
  if (str == NULL) return NULL;
  unsigned long len = strlen(str);
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(char) * len + 1);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memcpy(pie->buf, str, sizeof(char) * len + 1);
  pie->type = IT_STRING;
  pie->len = sizeof(char) * len;
  return pie;
}

IndraEnt *itCreateStringByLength(unsigned long strLength) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(strLength + 1);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memset(pie->buf, 0, strLength + 1);
  pie->type = IT_STRING;
  pie->len = strLength;
  return pie;
}

IndraEnt *itCreateStringFromSlice(const IndraEnt *pieByteString, unsigned long sliceStart, unsigned long sliceLength) {
  if (!pieByteString) return NULL;
  if (pieByteString->type != IT_STRING && pieByteString->type != IT_BYTES) return NULL;
  if (sliceStart + sliceLength > pieByteString->len) return NULL;
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sliceLength + 1);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memcpy(pie->buf, &(((char *)(pieByteString->buf))[sliceStart]), sliceLength);
  ((char *)(pie->buf))[sliceLength] = 0;
  pie->type = IT_STRING;
  pie->len = sliceLength;
  return pie;
}

IndraEnt *itCreateInt(int i) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(int));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(int *)pie->buf=i;
  pie->type = IT_INT;
  pie->len = sizeof(int);
  return pie;
}

IndraEnt *itCreateUInt(unsigned int ui) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned int));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned int *)pie->buf=ui;
  pie->type = IT_UINT;
  pie->len = sizeof(unsigned int);
  return pie;
}

IndraEnt *itCreateLong(long l) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(long));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(long *)pie->buf=l;
  pie->type = IT_LONG;
  pie->len = sizeof(long);
  return pie;
}

IndraEnt *itCreateULong(unsigned long ul) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned long));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned long *)pie->buf=ul;
  pie->type = IT_ULONG;
  pie->len = sizeof(unsigned long);
  return pie;
}

IndraEnt *itCreateFloat(float f) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(float));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(float *)pie->buf=f;
  pie->type = IT_FLOAT;
  pie->len = sizeof(float);
  return pie;
}

IndraEnt *itCreateDouble(double df) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(double));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(double *)pie->buf=df;
  pie->type = IT_DOUBLE;
  pie->len = sizeof(double);
  return pie;
}

void itPrint(const IndraEnt *pie) {
  if (pie == NULL) {
    printf("<NULL>");
    return;
  }
  char buf[10];
  switch (pie->type) {
  case IT_NIL:
    printf("<NIL>");
    break;
  case IT_CHAR:
    printf("%c", *(char *)pie->buf);
    break;
  case IT_BYTE:
    printf("0x%02x", *(unsigned char *)pie->buf);
    break;
  case IT_BYTES:
    printf("0x0000  ");
    for (unsigned long i=0; i<pie->len; i++) {
      printf("0x%02x ", ((unsigned char *)pie->buf)[i]);
      if ((i+1)%8 == 0) printf("\n0x%04x  ", (unsigned int)(i%0x10000));
    }
    printf("\n");
    break;
  case IT_STRING:
    printf("%s", (char *)pie->buf);
    break;
  case IT_INT:
    printf("%d", *(int *)pie->buf);
    break;
  case IT_UINT:
    printf("%u", *(unsigned int *)pie->buf);
    break;
  case IT_LONG:
    printf("%ld", *(long *)pie->buf);
    break;
  case IT_ULONG:
    printf("%lu", *(unsigned long *)pie->buf);
    break;
  case IT_FLOAT:
    printf("%f", *(float *)pie->buf);
    break;
  case IT_DOUBLE:
    printf("%lf", *(double *)pie->buf);
    break;
  case IT_INFNUM:
    printf("<NOT_IMPL>");
    break;
  }
}

void itPrintLn(const IndraEnt *pie) {
  itPrint(pie);
  printf("\n");
}

// ======== Arrays ==================================================
void itArrayPrint(const IndraEntArray *piea) {
  if (piea == NULL) {
    printf("<NULL>");
  }
  printf("[");
  bool first=true;
  for (unsigned long i=0; i<piea->count; i++) {
    if (first) first=false;
    else printf(", ");
    if (piea->type == IT_STRING) printf("\"");
    itPrint(&(piea->ieArray[i]));
    if (piea->type == IT_STRING) printf("\"");
  }
  printf("]");
}

void itArrayPrintLn(const IndraEntArray *piea) {
  itArrayPrint(piea);
  printf("\n");
}

IndraEntArray *itArrayCreate(IndraTypes type, unsigned long capacity) {
  IndraEntArray *piea = (IndraEntArray *)malloc(sizeof(IndraEntArray));
  memset(piea, 0, sizeof(IndraEntArray));
  piea->type = type;
  piea->capacity = capacity;
  if (capacity>0) {
    piea->ieArray = (IndraEnt *)malloc(sizeof(IndraEnt)*capacity);
    for (unsigned long i=0; i<capacity; i++) {
      memset(&piea->ieArray[i], 0, sizeof(IndraEnt));
      piea->ieArray[i].type = type;
    }
  }
  return piea;
}

void itArrayDelete(IndraEntArray *piea) {
  if (piea == NULL) return;
  if (piea->ieArray == NULL) {
    free(piea);
    return;
  }
  for (unsigned long i=0; i< piea->count; i++) {
    if (piea->ieArray[i].buf != NULL) {
      free(piea->ieArray[i].buf);
    }
  }
  free(piea->ieArray);
  free(piea);
  return;
}

IndraEntArray *itArrayResize(IndraEntArray **oldArray, unsigned long capacity) {
  if (oldArray == NULL) return NULL;
  if (capacity < (*oldArray)->count) return NULL;
  IndraEntArray *pieaNew = itArrayCreate((*oldArray)->type, capacity);
  if (pieaNew == NULL) return NULL;
  memcpy(pieaNew->ieArray, (*oldArray)->ieArray, (*oldArray)->count * sizeof(IndraEnt));
  pieaNew->count = (*oldArray)->count;
  free((*oldArray)->ieArray);
  free(*oldArray);
  *oldArray = NULL;
  return pieaNew;
}

IndraEnt *itArrayGet(const IndraEntArray *piea, unsigned long index) {
  if (piea==NULL) return NULL;
  if (piea->count <= index) return NULL;
  return &piea->ieArray[index];
}

bool itArraySet(IndraEntArray *piea, unsigned long index, IndraEnt *pie) {
  if (piea == NULL) return false;
  if (pie == NULL) return false;
  if (piea->capacity <= index) return false;
  if (piea->type != pie->type) return false;
  piea->ieArray[index] = *pie;
  pie->buf = NULL; // That buffer is now owned by the array. TBD?
  pie->len = 0;
  pie->type = IT_NIL;
  if (index + 1 > piea->count) {
    piea->count = index + 1;
  }
  return true;
}

bool itArraySetGrow(IndraEntArray **piea, unsigned long index, IndraEnt *pie) {
  if (piea == NULL || *piea == NULL) return false;
  if (pie == NULL) return false;
  if ((*piea)->capacity <= index) {
    unsigned long new_capa = index * 3 / 2;
    // printf("Growing array 1 to %lu\n", new_capa);
    *piea = itArrayResize(piea, new_capa);
    if (*piea == NULL) return false;
  }
  if ((*piea)->type != pie->type) return false;
  (*piea)->ieArray[index] = *pie;
  pie->buf = NULL; // That buffer is now owned by the array. TBD?
  pie->len = 0;
  pie->type = IT_NIL;
  if (index + 1 > (*piea)->count) {
    (*piea)->count = index + 1;
  }
  return true;
}

bool itArrayAppend(IndraEntArray **piea, IndraEnt *pie) {
  return itArraySetGrow(piea, (*piea)->count, pie);
}

bool itArrayRemove(IndraEntArray *piea, unsigned long index) {
  if (piea==NULL) return false;
  if (piea->count <= index) return false;
  if (piea->ieArray[index].buf != NULL) free(piea->ieArray[index].buf);
  memmove(&piea->ieArray[index],&piea->ieArray[index+1], (piea->count - index -1)*sizeof(IndraEnt)); 
  //for (unsigned long i=index; i<piea->count-1; i++) {
  //  piea->ieArray[i] = piea->ieArray[i+1];
  //}
  piea->count -= 1;
  return true;
}

// pie->buf gets NULLed!
bool itArrayInsert(IndraEntArray **ppiea, unsigned long index, IndraEnt *pie) {
  if (ppiea==NULL || *ppiea==NULL) return false;
  if ((*ppiea)->count <= index) {
    return itArraySetGrow(ppiea, index, pie);
  }
  if ((*ppiea)->capacity <= index) {
    unsigned long new_capa = index * 3 / 2;
    // printf("Growing array 2 to %lu\n", new_capa);
    *ppiea = itArrayResize(ppiea, new_capa);
    if (*ppiea == NULL) return false;
  }
  if ((*ppiea)->count+1 >= (*ppiea)->capacity) {
    unsigned long new_capa = (*ppiea)->count * 3 / 2;
    // printf("Growing array 3 to %lu\n", new_capa);
    *ppiea = itArrayResize(ppiea, new_capa);
    if (*ppiea == NULL) return false;
  }
  memmove(&(*ppiea)->ieArray[index+1], &(*ppiea)->ieArray[index], ((*ppiea)->count - index)*sizeof(IndraEnt));
  //for (unsigned long i=(*ppiea)->count; i>index; i--) {
  //  (*ppiea)->ieArray[i] = (*ppiea)->ieArray[i-1];
  //}
  (*ppiea)->ieArray[index] = *pie;
  pie->buf = NULL;
  (*ppiea)->count += 1;
  return true;
}

// ======== Hash-Map (slow write!) ================================================
IndraEntMap *itMapCreateHash(IndraTypes keyType, IndraTypes valueType, IndraHashTypes hashType) {
  IndraEntMap *piem = (IndraEntMap *)malloc(sizeof(IndraEntMap));
  if (piem == NULL) return NULL;
  piem->hashType = hashType;
  piem->pHash = itArrayCreate(IT_ULONG, 4);
  if (piem->pHash==NULL) {
    free(piem);
    return NULL;
  }
  piem->pKeys = itArrayCreate(keyType, 4);
  if (piem->pKeys == NULL) {
    free(piem->pHash);
    free(piem);
    return NULL;
  }
  piem->pValues = itArrayCreate(valueType, 4);
  if (piem->pValues == NULL) {
    free(piem->pHash);
    free(piem->pKeys);
    free(piem);
    return NULL;
  }
  return piem;
}

IndraEntMap *itMapCreate(IndraTypes keyType, IndraTypes valueType) {
  return itMapCreateHash(keyType, valueType, IT_HASH_SIMPLE);
}

void itMapDelete(IndraEntMap *piem) {
  if (piem == NULL) return;
  itArrayDelete(piem->pHash);
  itArrayDelete(piem->pKeys);
  itArrayDelete(piem->pValues);
  free(piem);
}

unsigned long _itGetHash(IndraHashTypes hashType, unsigned char *buf, unsigned long len) {
  unsigned long hash;
  switch (hashType) {
  case IT_HASH_CRC16:
    hash = (unsigned long)itCrc16Ccitt(buf, len);
    break;
  case IT_HASH_SIMPLE:
    hash = itSimpleHash(buf, len);
    break;
  }
  return hash;
}

// Return index of 'left-most' matching hash, if hash is found,
// If index < 0, Insertion-point would be at -index -1 (Insertion point can be beyond current len!)
long _itMapHashIndexGet(IndraEntMap *piem, IndraEnt *pKey, unsigned long *pHash) {
  *pHash = _itGetHash(piem->hashType, pKey->buf, pKey->len);
  unsigned long n = piem->pHash->count;
  unsigned long l = 0;
  unsigned long r = n;
  unsigned long *pCurHash = NULL;
  unsigned long m = -1;
  while (l<r) {
    m = (l+r)/2;
    pCurHash = piem->pHash->ieArray[m].buf;
    if (pCurHash != NULL) {
      if (*pCurHash < *pHash) {
        l = m+1;
      } else {
        r = m;
      }
    } else {
      l=0;
      break;
    }
  }
  if (l>=n) {
    return -l-1;  // Not found, upper end, return negative insertion point. Insert at -l-1.
  } else {
    pCurHash = (unsigned long *)(piem->pHash->ieArray[l].buf);
    if (*pCurHash != *pHash) {
      return -l-1;  // Not found, return negative insertion point. Insert at -l-1.
    }
  } 
  return l;
}

long _itMapIndexGet(IndraEntMap *piem, IndraEnt *pKey, unsigned long *pHash) {
  long index = _itMapHashIndexGet(piem, pKey, pHash);
  if (index >= 0) { // Key might exist
    unsigned long curInd = index;
    while (curInd<piem->pKeys->count && *(unsigned long *)(piem->pHash->ieArray[curInd].buf) == *pHash && piem->pKeys->ieArray[curInd].len == pKey->len) {
      if (!memcmp(piem->pKeys->ieArray[curInd].buf, pKey->buf, pKey->len)) {
        return curInd;
      } else {
        curInd += 1;
      }
    }
  }
  return -1; 
}

bool itMapSet(IndraEntMap *piem, IndraEnt *pKey, IndraEnt *pValue) {
  if (piem==NULL || pKey==NULL || pValue==NULL) return false;
  unsigned long hash;
  long index = _itMapHashIndexGet(piem, pKey, &hash);
  bool exists = false;
  long insertionIndex= -1;
  if (index >= 0) { // Key might exist
    unsigned long curInd = index;
    insertionIndex = index;
    while (curInd<piem->pKeys->count && *(unsigned long *)(piem->pHash->ieArray[curInd].buf) == hash && piem->pKeys->ieArray[curInd].len == pKey->len) {
      if (!memcmp(piem->pKeys->ieArray[curInd].buf, pKey->buf, pKey->len)) {
        exists = true;
        index = curInd;
        break;
      } else {
        curInd += 1;
      }
    }
  } else {
    insertionIndex = -index - 1;
  }
  if (exists) {
    free(piem->pValues->ieArray[index].buf);
    piem->pValues->ieArray[index] = *pValue;
    pValue->buf = NULL;
  } else {
    // unsigned long hash = _itGetHash(piem->hashType, pKey->buf, pKey->len);
    IndraEnt *pHash = itCreateULong(hash);
    itArrayInsert(&(piem->pHash), insertionIndex, pHash);
    itDelete(pHash);
    itArrayInsert(&(piem->pKeys), insertionIndex, pKey);
    itArrayInsert(&(piem->pValues), insertionIndex, pValue);
  }
  return true;
}

IndraEnt* itMapGet(IndraEntMap *piem, IndraEnt *pKey) {
  unsigned long hash;
  long index = _itMapIndexGet(piem, pKey, &hash);
  if (index < 0) return NULL;
  return &(piem->pValues->ieArray[index]);
}

bool itMapExists(IndraEntMap *piem, IndraEnt *pKey) {
  unsigned long hash;
  if (_itMapIndexGet(piem, pKey, &hash) >= 0) return true;
  return false;
}

bool itMapRemove(IndraEntMap *piem, IndraEnt *pKey) {
  unsigned long hash;
  long index = _itMapIndexGet(piem, pKey, &hash);
  if (index < 0) return false;
  itArrayRemove(piem->pHash, index);
  itArrayRemove(piem->pKeys, index);
  itArrayRemove(piem->pValues, index);
  return false;
}

void itMapPrint(IndraEntMap *piem) {
  if (piem==NULL) {
    printf("<NULL");
    return;
  }
  for (unsigned long i=0; i<piem->pHash->count; i++) {
    printf("[%lu]: >", *(unsigned long *)(piem->pHash->ieArray[i].buf)); itPrint(&(piem->pKeys->ieArray[i]));
    printf("< => >"); itPrint(&(piem->pValues->ieArray[i])); printf("<\n");
  }
}

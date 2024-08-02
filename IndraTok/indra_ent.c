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
    //printf("Growing array to %lu\n", new_capa);
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
  for (unsigned long i=index; i<piea->count-1; i++) {
    piea->ieArray[i] = piea->ieArray[i+1];
  }
  piea->count -= 1;
  return true;
}

bool itArrayInsert(IndraEntArray *piea, unsigned long index, IndraEnt *pie) {
  if (piea==NULL) return false;
  if (piea->count <= index) return false;
  if (piea->count+1 >= piea->capacity) return false;
  for (unsigned long i=piea->count; i>index; i--) {
    piea->ieArray[i] = piea->ieArray[i-1];
  }
  piea->ieArray[index] = *pie;
  piea->count += 1;
  return true;
}

// ======== uHash-MAP ================================================
typedef struct _u_hash_entry {
  unsigned long keySize;
  void *pKey;
  unsigned long valueSize;
  void *pValue;
  unsigned long hash;
} uHashEntry;

typedef struct _u_hash {
  unsigned long size;
  unsigned long entries;
  uHashEntry *pHE;
} uHash;

uHash* uHashCreate(unsigned long size);
void uHashDelete(uHash *puHash);
uHash* uHashGrow(uHash *puHash, unsigned long newSize);

IndraEntMap *itCreateMap(IndraTypes keyType, IndraTypes valueType) {
  IndraEntMap *piem = (IndraEntMap *)malloc(sizeof(IndraEntMap));
  if (piem == NULL) return NULL;
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

void itDeleteMap(IndraEntMap *piem) {
  if (piem == NULL) return;
  itArrayDelete(piem->pHash);
  itArrayDelete(piem->pKeys);
  itArrayDelete(piem->pValues);
  free(piem);
}


long uHashIndex(uHash *puHash, const void *pKey, unsigned long keySize) {
  if (puHash->entries == 0) return -1;
  unsigned long hash = (unsigned long)itCrc16Ccitt((const uint8_t *)pKey, keySize);
  unsigned long a=0, d=0;
  unsigned long b=puHash->entries-1;
  while (b-a >= 1) {
    d = (a+b)/2;  // a + (b-a)/2;  
    if (puHash->pHE[a].hash==hash) break;
    if (puHash->pHE[b].hash==hash) {
      a=b;
      break;
    }
    if (puHash->pHE[d].hash==hash) {
      a=d;
      break;
    }
    if (puHash->pHE[d].hash < hash) {
      if (b==d) {
        a=d;
        break;
      }
      b = d;
    } else {
      if (a==d) break;
      a = d;
    }
  }
  while (puHash->pHE[a].hash == hash) {
    if (puHash->pHE[a].keySize == keySize && !memcmp(puHash->pHE[a].pKey, pKey, keySize)) return a;
    ++a;
    if (a>=puHash->entries) break;
  }  
  return -a - 1;
}

void uHashInsert(uHash *puHash, const void *pKey, unsigned long keySize, const void *pValue, unsigned long valueSize) {
  if (!puHash || !pKey || !keySize || !pValue || !valueSize) return;
  if (puHash->size == puHash->entries) return;
  long idx, ins_idx;
  idx = uHashIndex(puHash, pKey, keySize);
  if (idx>=0) {
    // Existing key
    ins_idx = idx;
    free(puHash->pHE[ins_idx].pKey);
    free(puHash->pHE[ins_idx].pValue);
  } else {
    // New key
    ins_idx = -idx -1; 
    for (unsigned long li=puHash->entries; li>ins_idx; li--) {
      puHash->pHE[li] = puHash->pHE[li-1];
    }
    ++(puHash->entries);
  }
  puHash->pHE[ins_idx].hash = itCrc16Ccitt((const uint8_t *)pKey, keySize);
  puHash->pHE[ins_idx].pKey = malloc(keySize);
  memcpy(puHash->pHE[ins_idx].pKey, pKey, keySize);
  puHash->pHE[ins_idx].keySize = keySize;
  puHash->pHE[ins_idx].pValue = malloc(valueSize);
  memcpy(puHash->pHE[ins_idx].pValue, pValue, valueSize);
  puHash->pHE[ins_idx].valueSize = valueSize;
}


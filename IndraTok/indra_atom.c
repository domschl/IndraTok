#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_atom.h"
//#include "indra_crc_crypt.h"


void iaDelete(IndraAtom *pia) {
  if (pia == NULL) return;
  if (pia->type != IA_NIL && pia->buf != NULL) {
    free(pia->buf);
  }
  free(pia);
}

IndraAtom *iaCreateChar(char c) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(char));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(char *)pia->buf=c;
  pia->type = IA_CHAR;
  pia->recsize = indraTypesRecsize[IA_CHAR];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateByte(unsigned char byte) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(unsigned char));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(unsigned char *)pia->buf=byte;
  pia->type = IA_BYTE;
  pia->recsize = indraTypesRecsize[IA_BYTE];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateBytes(const unsigned char bytes[], unsigned long length) {
  if (bytes == NULL) return NULL;
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(unsigned char) * length);
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  memcpy(pia->buf, bytes, sizeof(unsigned char) * length);
  pia->type = IA_BYTE;
  pia->recsize = indraTypesRecsize[IA_BYTE];
  pia->capacity = length;
  pia->count = length;
  return pia;
}

IndraAtom *iaCreateString(const char str[]) {
  if (str == NULL) return NULL;
  unsigned long length = strlen(str);
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(char) * length + 1);
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  memcpy(pia->buf, str, sizeof(char) * length + 1);
  pia->type = IA_CHAR;
  pia->recsize = indraTypesRecsize[IA_CHAR];
  pia->capacity = length + 1;
  pia->count = length;
  return pia;
}

IndraAtom *iaCreateStringFromSlice(const IndraAtom *pia, unsigned long sliceStart, unsigned long sliceLength) {
  if (!pia) return NULL;
  if (pia->type != IA_CHAR && pia->type != IA_BYTE) return NULL;
  if (sliceStart + sliceLength > pia->count) return NULL;
  IndraAtom *pias = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pias) return NULL;
  memset(pias, 0, sizeof(IndraAtom));
  pias->buf=malloc(sliceLength + 1);
  if (!pias->buf) {
    free(pias);
    return NULL;
  }
  memcpy(pias->buf, &(((char *)(pia->buf))[sliceStart]), sliceLength);
  ((char *)(pias->buf))[sliceLength] = 0;
  pias->type = IA_CHAR;
  pias->recsize = indraTypesRecsize[IA_CHAR];
  pias->capacity = sliceLength + 1;
  pias->count = sliceLength;
  return pias;
}

IndraAtom *iaCreateInt(int i) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(int));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(int *)pia->buf=i;
  pia->type = IA_INT;
  pia->recsize = indraTypesRecsize[IA_INT];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateUInt(unsigned int ui) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(unsigned int));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(unsigned int *)pia->buf=ui;
  pia->type = IA_UINT;
  pia->recsize = indraTypesRecsize[IA_UINT];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateLong(long l) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(long));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(long *)pia->buf=l;
  pia->type = IA_LONG;
  pia->recsize = indraTypesRecsize[IA_LONG];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateULong(unsigned long ul) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(unsigned long));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(unsigned long *)pia->buf=ul;
  pia->type = IA_ULONG;
  pia->recsize = indraTypesRecsize[IA_ULONG];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateFloat(float f) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(float));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(float *)pia->buf=f;
  pia->type = IA_FLOAT;
  pia->recsize = indraTypesRecsize[IA_FLOAT];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreateDouble(double df) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  if (!pia) return NULL;
  memset(pia, 0, sizeof(IndraAtom));
  pia->buf=malloc(sizeof(double));
  if (!pia->buf) {
    free(pia);
    return NULL;
  }
  *(double *)pia->buf=df;
  pia->type = IA_DOUBLE;
  pia->recsize = indraTypesRecsize[IA_DOUBLE];
  pia->capacity = 1;
  pia->count = 1;
  return pia;
}

IndraAtom *iaCreate(IndraTypes type, void *buf, unsigned long count, unsigned long capacity) {
  IndraAtom *pia = (IndraAtom *)malloc(sizeof(IndraAtom));
  memset(pia, 0, sizeof(IndraAtom));
  pia->type = type;
  pia->recsize = indraTypesRecsize[type];
  pia->count = count;
  unsigned long len = pia->recsize * count;
  pia->capacity = pia->count;
  pia->buf = malloc(pia->recsize * pia->capacity);
  memcpy(pia->buf, buf, pia->recsize * pia->count);
  return pia;
}

void iaPrint(const IndraAtom *pia) {
  if (pia == NULL) {
    printf("<NULL>");
    return;
  }
  if (pia->count > 1) {
    if (pia->type==IA_CHAR) printf("\"");
    else printf("[");
  }
  for (unsigned long i=0; i<pia->count; i++) {
    switch (pia->type) {
    case IA_NIL:
      printf("<NIL>");
      break;
    case IA_CHAR:
      printf("%c", ((char *)pia->buf)[i]);
      break;
    case IA_BYTE:
      printf("0x%02x", ((unsigned char *)pia->buf)[i]);
      break;
    case IA_INT:
      printf("%d", *(int *)pia->buf);
      break;
    case IA_UINT:
      printf("%u", *(unsigned int *)pia->buf);
      break;
    case IA_LONG:
      printf("%ld", *(long *)pia->buf);
      break;
    case IA_ULONG:
      printf("%lu", *(unsigned long *)pia->buf);
      break;
    case IA_FLOAT:
      printf("%f", *(float *)pia->buf);
      break;
    case IA_DOUBLE:
      printf("%lf", *(double *)pia->buf);
      break;
    case IA_INFNUM:
      printf("<NOT_IMPL>");
      break;
    case IA_ATOM:
      printf("[");
      for (uint j=0; j<pia->count; j++) {
        iaPrint(&((const IndraAtom *)pia->buf)[j]);
        if (j!=pia->count -1) printf(", ");
      }
      printf("]");
      break;
    case IA_MAP:
    case IA_LIST:
      printf("<NOT_IMPL>");
      break;    
    }
    if (pia->count > 1 && i != pia->count-1) {
      if (pia->type != IA_CHAR) printf(", ");
    }
  }
  if (pia->count > 1) {
    if (pia->type==IA_CHAR) printf("\"");
    else printf("]");
  }
}

void iaPrintLn(const IndraAtom *pia) {
  iaPrint(pia);
  printf("\n");
}

// ======== Arrays ==================================================
IndraAtom *iaResize(IndraAtom **ppiaOld, unsigned long capacity) {
  if (ppiaOld == NULL || *ppiaOld==NULL) return NULL;
  if (capacity < (*ppiaOld)->count) return NULL;
  IndraAtom *piaNew = iaCreate((*ppiaOld)->type, (*ppiaOld)->buf, (*ppiaOld)->count, capacity);
  if (piaNew == NULL) return NULL;
  free((*ppiaOld)->buf);
  free(*ppiaOld);
  *ppiaOld = NULL;
  return piaNew;
}

void *iaArrayGet(const IndraAtom *pia, unsigned long index) {
  if (pia==NULL) return NULL;
  if (pia->count <= index) return NULL;
  return &pia->buf[index*pia->recsize];
}

bool iaArraySet(IndraAtom *pia, unsigned long index, const void *buf) {
  if (pia == NULL) return false;
  if (buf == NULL) return false;
  if (pia->capacity <= index) return false;
  memcpy(&pia->buf[index*pia->recsize], buf, pia->recsize);
  if (index + 1 > pia->count) {
    pia->count = index + 1;
  }
  return true;
}

bool iaArraySetGrow(IndraAtom **ppia, unsigned long index, const void *buf) {
  if (ppia == NULL || *ppia == NULL) return false;
  if (buf == NULL) return false;
  if ((*ppia)->capacity <= index) {
    unsigned long new_capa = index * 2;
    // printf("Growing array 1 to %lu\n", new_capa);
    *ppia = iaResize(ppia, new_capa);
    if (*ppia == NULL) return false;
  }
  return iaArraySet(*ppia, index, buf);
}

bool iaArrayAppend(IndraAtom **ppia, const void *buf) {
  return iaArraySetGrow(ppia, (*ppia)->count, buf);
}

bool iaJoin(IndraAtom **ppiaRoot, const IndraAtom *piaAppendix) {
  if (ppiaRoot==NULL || *ppiaRoot==NULL || piaAppendix==NULL) return false;
  if ((*ppiaRoot)->type != piaAppendix->type) return false;
  unsigned long old_count = (*ppiaRoot)->count;
  unsigned long new_count = old_count + piaAppendix->count;
  unsigned long new_capa = old_count + piaAppendix->count;
  if ((*ppiaRoot)->type == IA_CHAR) new_capa += 1; 
  if (new_capa > (*ppiaRoot)->capacity) {
    *ppiaRoot = iaResize(ppiaRoot, new_capa);
    if (*ppiaRoot == NULL) return false;
  }
  memcpy(&(*ppiaRoot)->buf[old_count * (*ppiaRoot)->recsize], piaAppendix->buf, piaAppendix->count * (*ppiaRoot)->recsize);
  (*ppiaRoot)->count = new_count;
  if ((*ppiaRoot)->type == IA_CHAR) ((char *)(*ppiaRoot)->buf)[new_count] = 0;
  return true;
}

IndraAtom *iaSlice(IndraAtom *pia, unsigned long index, unsigned long count) {
  if (pia == NULL) return NULL;
  if (index + count > pia->count) return NULL;
  unsigned long capa = count;
  if (pia->type == IA_CHAR) capa += 1;
  IndraAtom* piaSlice = iaCreate(pia->type, &(pia->buf[index * pia->recsize]), count, capa);
  if (piaSlice->type == IA_CHAR) ((char *)piaSlice->buf)[piaSlice->count]=0;
  return piaSlice;
}

bool iaArrayRemove(IndraAtom *pia, unsigned long index) {
  if (pia==NULL) return false;
  if (pia->count <= index) return false;
  memmove(&pia->buf[pia->recsize*index],&pia->buf[pia->recsize * (index+1)], (pia->recsize *(pia->count - index -1))); 
  pia->count -= 1;
  return true;
}

bool iaArrayInsert(IndraAtom **ppia, unsigned long index, const void *buf) {
  if (ppia==NULL || *ppia==NULL) return false;
  if (buf == NULL) return false;
  if ((*ppia)->count <= index) {
    return iaArraySetGrow(ppia, index, buf);
  }
  if ((*ppia)->capacity <= (*ppia)->count) {
    unsigned long new_capa = (*ppia)->capacity * 2;
    // printf("Growing array 2 to %lu\n", new_capa);
    *ppia = iaResize(ppia, new_capa);
    if (*ppia == NULL) return false;
  }
  /*
  if ((*ppiaa)->count+1 >= (*ppiaa)->capacity) {
    unsigned long new_capa = (*ppiaa)->count * 3 / 2;
    // printf("Growing array 3 to %lu\n", new_capa);
    *ppiaa = iaArrayResize(ppiaa, new_capa);
    if (*ppiaa == NULL) return false;
  }
  */
  memmove(&(*ppia)->buf[(*ppia)->recsize *(index+1)], &(*ppia)->buf[(*ppia)->recsize * index], ((*ppia)->count - index)*(*ppia)->recsize);
  //for (unsigned long i=(*ppiaa)->count; i>index; i--) {
  //  (*ppiaa)->ieArray[i] = (*ppiaa)->ieArray[i-1];
  //}
  memcpy(&(*ppia)->buf[(*ppia)->recsize * index], buf, (*ppia)->recsize);
  (*ppia)->count += 1;
  return true;
}
/*
// ======== Old Hash-Map (slow write!) ================================================
IndraOldEntMap *iaOldMapCreateHash(IndraTypes keyType, IndraTypes valueType, IndraHashTypes hashType) {
  IndraOldEntMap *piam = (IndraOldEntMap *)malloc(sizeof(IndraOldEntMap));
  if (piam == NULL) return NULL;
  piam->hashType = hashType;
  piam->pHash = iaArrayCreate(IT_ULONG, 4);
  if (piam->pHash==NULL) {
    free(piam);
    return NULL;
  }
  piam->pKeys = iaArrayCreate(keyType, 4);
  if (piam->pKeys == NULL) {
    free(piam->pHash);
    free(piam);
    return NULL;
  }
  piam->pValues = iaArrayCreate(valueType, 4);
  if (piam->pValues == NULL) {
    free(piam->pHash);
    free(piam->pKeys);
    free(piam);
    return NULL;
  }
  return piam;
}

IndraOldEntMap *iaOldMapCreate(IndraTypes keyType, IndraTypes valueType) {
  return iaOldMapCreateHash(keyType, valueType, IA_HASH_SIMPLE);
}

void iaOldMapDelete(IndraOldEntMap *piam) {
  if (piam == NULL) return;
  iaArrayDelete(piam->pHash);
  iaArrayDelete(piam->pKeys);
  iaArrayDelete(piam->pValues);
  free(piam);
}

unsigned long _itGetHash(IndraHashTypes hashType, unsigned char *buf, unsigned long len) {
  unsigned long hash;
  switch (hashType) {
  case IA_HASH_CRC16:
    hash = (unsigned long)itCrc16Ccitt(buf, len);
    break;
  case IA_HASH_SIMPLE:
    hash = iaSimpleHash(buf, len);
    break;
  }
  return hash;
}

// Return index of 'left-most' matching hash, if hash is found,
// If index < 0, Insertion-point would be at -index -1 (Insertion point can be beyond current len!)
long _itOldMapHashIndexGet(IndraOldEntMap *piam, IndraAtom *pKey, unsigned long *pHash) {
  *pHash = _itGetHash(piam->hashType, pKey->buf, pKey->len);
  unsigned long n = piam->pHash->count;
  unsigned long l = 0;
  unsigned long r = n;
  unsigned long *pCurHash = NULL;
  unsigned long m = -1;
  while (l<r) {
    m = (l+r)/2;
    pCurHash = piam->pHash->ieArray[m].buf;
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
    pCurHash = (unsigned long *)(piam->pHash->ieArray[l].buf);
    if (*pCurHash != *pHash) {
      return -l-1;  // Not found, return negative insertion point. Insert at -l-1.
    }
  } 
  return l;
}

long _itOldMapIndexGet(IndraOldEntMap *piam, IndraAtom *pKey, unsigned long *pHash) {
  long index = _itOldMapHashIndexGet(piam, pKey, pHash);
  if (index >= 0) { // Key might exist
    unsigned long curInd = index;
    while (curInd<piam->pKeys->count && *(unsigned long *)(piam->pHash->ieArray[curInd].buf) == *pHash && piam->pKeys->ieArray[curInd].len == pKey->len) {
      if (!memcmp(piam->pKeys->ieArray[curInd].buf, pKey->buf, pKey->len)) {
        return curInd;
      } else {
        curInd += 1;
      }
    }
  }
  return -1; 
}

bool iaOldMapSet(IndraOldEntMap *piam, IndraAtom *pKey, IndraAtom *pValue) {
  if (piam==NULL || pKey==NULL || pValue==NULL) return false;
  unsigned long hash;
  long index = _itOldMapHashIndexGet(piam, pKey, &hash);
  bool exists = false;
  long insertionIndex= -1;
  if (index >= 0) { // Key might exist
    unsigned long curInd = index;
    insertionIndex = index;
    while (curInd<piam->pKeys->count && *(unsigned long *)(piam->pHash->ieArray[curInd].buf) == hash && piam->pKeys->ieArray[curInd].len == pKey->len) {
      if (!memcmp(piam->pKeys->ieArray[curInd].buf, pKey->buf, pKey->len)) {
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
    free(piam->pValues->ieArray[index].buf);
    piam->pValues->ieArray[index] = *pValue;
    pValue->buf = NULL;
  } else {
    // unsigned long hash = _itGetHash(piam->hashType, pKey->buf, pKey->len);
    IndraAtom *pHash = iaCreateULong(hash);
    iaArrayInsert(&(piam->pHash), insertionIndex, pHash);
    iaDelete(pHash);
    iaArrayInsert(&(piam->pKeys), insertionIndex, pKey);
    iaArrayInsert(&(piam->pValues), insertionIndex, pValue);
  }
  return true;
}

IndraAtom* iaOldMapGet(IndraOldEntMap *piam, IndraAtom *pKey) {
  unsigned long hash;
  long index = _itOldMapIndexGet(piam, pKey, &hash);
  if (index < 0) return NULL;
  return &(piam->pValues->ieArray[index]);
}

bool iaOldMapExists(IndraOldEntMap *piam, IndraAtom *pKey) {
  unsigned long hash;
  if (_itOldMapIndexGet(piam, pKey, &hash) >= 0) return true;
  return false;
}

bool iaOldMapRemove(IndraOldEntMap *piam, IndraAtom *pKey) {
  unsigned long hash;
  long index = _itOldMapIndexGet(piam, pKey, &hash);
  if (index < 0) return false;
  iaArrayRemove(piam->pHash, index);
  iaArrayRemove(piam->pKeys, index);
  iaArrayRemove(piam->pValues, index);
  return false;
}

void iaOldMapPrint(IndraOldEntMap *piam) {
  if (piam==NULL) {
    printf("<NULL");
    return;
  }
  for (unsigned long i=0; i<piam->pHash->count; i++) {
    printf("[%lu]: >", *(unsigned long *)(piam->pHash->ieArray[i].buf)); iaPrint(&(piam->pKeys->ieArray[i]));
    printf("< => >"); iaPrint(&(piam->pValues->ieArray[i])); printf("<\n");
  }
}
*/

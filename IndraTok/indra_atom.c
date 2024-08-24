#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "indra_atom.h"
#include "indra_crc_crypt.h"

const unsigned long iaStackMax[] = {0, IA_STACK_CHARS, IA_STACK_WORDS, IA_STACK_INTS, IA_STACK_LONGS, IA_STACK_FLOATS, IA_STACK_DOUBLES, 0, 0};
const unsigned long iaTypesize[] = {0, sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t), sizeof(float), sizeof(double), sizeof(struct _ia_atom), sizeof(void *)};

unsigned long _getNextLargestPowerOf2(unsigned long n) {
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;
  n++;
  return n;
}

size_t iaGetRecsize(const IA_T_ATOM *pAtom) {
  if (pAtom->onHeap) {
    return pAtom->data.pHeap->recsize;
  } else {
    return iaTypesize[pAtom->type];
  }
}

void iaDelete(IA_T_ATOM *pAtom) {
  if (pAtom->type == IA_ID_ATOM) {
    for (uint64_t i=0; i<pAtom->count; i++) {
      IA_T_ATOM *pA = (IA_T_ATOM *)iaGetIndexPtr(pAtom, i);
      if (pA->onHeap) {
        iaDelete(pA);
      }
    }
    free(pAtom->data.pHeap);
  } else {
    if (pAtom->onHeap) {
      if (pAtom->data.pHeap) free(pAtom->data.pHeap);
      pAtom->data.pHeap = NULL;
    }  
  }
}

bool _recCopy(IA_T_ATOM *dest, IA_T_ATOM *src) {
  if (!src->onHeap) {
    *dest = *src;
  } else {
    dest->onHeap = 1;
    dest->type = src->type;
    dest->count = src->count;
    dest->data.pHeap = (IA_T_HEAP_HEADER *)malloc(sizeof(IA_T_HEAP_HEADER)+src->data.pHeap->recsize*src->count);
    if (dest->data.pHeap == NULL) {
      return false;
    }
    memcpy(dest->data.pHeap, src->data.pHeap, sizeof(IA_T_HEAP_HEADER));
    IA_T_ATOM *subdest = (IA_T_ATOM *)iaGetHeapDataPtr(dest);
    IA_T_ATOM *subsrc = (IA_T_ATOM *)iaGetHeapDataPtr(src);
    if (src->type == IA_ID_ATOM) {
      for (size_t i=0; i<src->count; i++ ) {
        subdest = iaGetIndexPtr(dest, i);
        subsrc = iaGetIndexPtr(src, i);
        memcpy(subdest, subsrc, src->data.pHeap->recsize);
        _recCopy(subdest, subsrc);
      }
    } else {
      memcpy(subdest, subsrc, src->data.pHeap->recsize*src->count);
    }
  }
  return true;
}

bool iaCreateCapacity(IA_T_ATOM *pAtom, int type, size_t recsize, size_t capacity, size_t count, void *pData) {
  memset(pAtom, 0, sizeof(IA_T_ATOM));
  pAtom->type = type;
  if (type == IA_ID_NIL) {
    pAtom->count = 0;
    return true;
  }
  pAtom->count = count;
  if (capacity > iaStackMax[type]) {
    pAtom->onHeap = 1;
    size_t new_capacity = 1;
    if (capacity > 1) {
      new_capacity = _getNextLargestPowerOf2(capacity);
      if (new_capacity < IA_MIN_NEW_CAPACITY) new_capacity = IA_MIN_NEW_CAPACITY;
    }
    pAtom->data.pHeap = (void *)malloc(sizeof(IA_T_HEAP_HEADER)+recsize*new_capacity);
    pAtom->data.pHeap->capacity = new_capacity;
    pAtom->data.pHeap->recsize = recsize;
    IA_T_ATOM *subatom = (IA_T_ATOM *)iaGetHeapDataPtr(pAtom);
    if (type == IA_ID_ATOM) {
      subatom->type = IA_ID_ATOM;
      subatom->count = count;
      for (size_t i=0; i<count; i++) {
        IA_T_ATOM *src = (IA_T_ATOM *)&(((unsigned char *)pData)[recsize * i]);  // iaGetIndexPtr(pData, i);
        IA_T_ATOM *dst = iaGetIndexPtr(pAtom, i);
        _recCopy(dst, src);
      }
      for (size_t i=count; i<new_capacity; i++) {
        IA_T_ATOM *dst = iaGetIndexPtr(pAtom, i);
        memset(dst, 0, sizeof(IA_T_ATOM));
        dst->type = IA_ID_ATOM;
      }
    } else {
      if (count) memcpy(subatom, pData, recsize*count);
    }
  } else {
    pAtom->onHeap = 0;
    uint8_t *pdest = iaGetStackDataPtr(pAtom);
    if (count * recsize>0) memcpy(pdest, pData, recsize*count);
  }
  return true;
}

bool iaCreate(IA_T_ATOM *pAtom, int type, size_t recsize, size_t count, void *pData) {
  memset(pAtom, 0, sizeof(IA_T_ATOM));
  pAtom->type = type;
  if (type == IA_ID_NIL) {
    pAtom->count = 0;
    return true;
  }
  pAtom->count = count;
  if (count > iaStackMax[type]) {
    pAtom->onHeap = 1;
    size_t new_capacity = 1;
    if (count > 1) {
      new_capacity = _getNextLargestPowerOf2(count);
      if (new_capacity < IA_MIN_NEW_CAPACITY) new_capacity = IA_MIN_NEW_CAPACITY;
    }
    pAtom->data.pHeap = (void *)malloc(sizeof(IA_T_HEAP_HEADER)+recsize*new_capacity);
    pAtom->data.pHeap->capacity = new_capacity;
    pAtom->data.pHeap->recsize = recsize;
    IA_T_ATOM *subatom = (IA_T_ATOM *)iaGetHeapDataPtr(pAtom);
    if (type == IA_ID_ATOM) {
      subatom->type = IA_ID_ATOM;
      subatom->count = count;
      for (size_t i=0; i<count; i++) {
        IA_T_ATOM *src = (IA_T_ATOM *)&(((unsigned char *)pData)[recsize * i]);  // iaGetIndexPtr(pData, i);
        IA_T_ATOM *dst = iaGetIndexPtr(pAtom, i);
        _recCopy(dst, src);
      }
    } else {
      memcpy(subatom, pData, recsize*count);
    }
  } else {
    pAtom->onHeap = 0;
    uint8_t *pdest = iaGetStackDataPtr(pAtom);
    if (count * recsize>0) memcpy(pdest, pData, recsize*count);
  }
  return true;
}

void iaSetChar(IA_T_ATOM *pAtom, uint8_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_CHAR;
  pAtom->count = 1;
  pAtom->data.c[0] = value;
}

bool iaSetString(IA_T_ATOM *pAtom, char *pString) {
  return iaCreate(pAtom, IA_ID_CHAR, sizeof(uint8_t), strlen(pString), pString);
} 

void iaSetWord(IA_T_ATOM *pAtom, uint16_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_CHAR;
  pAtom->count = 1;
  pAtom->data.w[0] = value;
}

void iaSetInt(IA_T_ATOM *pAtom, uint32_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_INT;
  pAtom->count = 1;
  pAtom->data.i[0] = value;
}

void iaSetLong(IA_T_ATOM *pAtom, uint64_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_LONG;
  pAtom->count = 1;
  pAtom->data.l[0] = value;
}

void iaSetFloat(IA_T_ATOM *pAtom, float value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_FLOAT;
  pAtom->count = 1;
  pAtom->data.f[0] = value;
}

void iaSetDouble(IA_T_ATOM *pAtom, double value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_DOUBLE;
  pAtom->count = 1;
  pAtom->data.d[0] = value;
}

void iaSetAtom(IA_T_ATOM *pAtom, IA_T_ATOM *pValue) {
  pAtom->onHeap = 1;
  pAtom->type = IA_ID_ATOM;
  pAtom->count = 1;
  pAtom->data.pHeap = malloc(sizeof(IA_T_HEAP_HEADER) + sizeof(IA_T_ATOM));
  memset(pAtom->data.pHeap, 0, sizeof(IA_T_HEAP_HEADER) + sizeof(IA_T_ATOM));
  pAtom->data.pHeap->capacity = 1;
  pAtom->data.pHeap->recsize = sizeof(IA_T_ATOM);
  IA_T_ATOM *pDest = iaGetDataPtr(pAtom);
  _recCopy(pDest, pValue);
}

void _ia_indent(int i) {
  for (int j=0; j<i; j++) {
    printf(" ");
  }
}

void _iaPrintRec(IA_T_ATOM *pAtom, int level) {
  IA_T_ATOM *pA;
  if (pAtom==NULL) {
    printf("<NULL>\n");
    return;
  }
  if (pAtom->count == 0) {
    // print empty set unicode symbol
    printf("\u2205");
    return;
  }
  if (level>5) {
    printf("\nRECURSION?\n");
    return;
  }
  if (pAtom->count > 100000) {
    printf("\nDATA CORRUPTION?\ncount=%ld, type=%d, onHeap=%d\n",pAtom->count, pAtom->type, pAtom->onHeap);
    return;
  }
  int maxIndent = 0;
  if (pAtom->count > 1) {
    printf("[");
  }
  // if (level > maxIndent+1) _ia_indent(level+1);
  for (size_t i=0; i<pAtom->count; i++) {
    void *pData = iaGetIndexPtr(pAtom, i);
    if (!pData) {
      printf("Error getting index %ld\n", i);
      return;
    }
    if (i>0 && i<pAtom->count) {
      if (pAtom->type != IA_ID_CHAR) {
        printf(", ");
      }
      //if (level==maxIndent+1) printf("\n");
    }
    
    switch (pAtom->type) {
    case IA_ID_CHAR:
      if (i==0) printf("\"");
      printf("%c", *(uint8_t *)pData);
      if (i==pAtom->count-1) printf("\"");
      break;
    case IA_ID_WORD:
      printf("%d", *(uint16_t *)pData);
      break;
    case IA_ID_INT:
      printf("%d", *(uint32_t *)pData);
      break;
    case IA_ID_LONG:
      printf("%ld", *(long *)pData);
      break;
    case IA_ID_FLOAT:
      printf("%f", *(float *)pData);
      break;
    case IA_ID_DOUBLE:
      printf("%f", *(double *)pData);
      break;
    case IA_ID_PANY:
        printf("Heap: %ld elements of type %d\n", (long)pAtom->count, pAtom->type);
      break;
    case IA_ID_ATOM:
      _iaPrintRec(pData, level+1);
      break;
    default:
      printf("Unknown type %d\n", pAtom->type);
      exit(-1);
      break;
    }
  }
  if (pAtom->count > 1) {
    printf("]");
  }
}

void iaPrint(IA_T_ATOM *pAtom) {
  _iaPrintRec(pAtom, 0);
}
  
void iaPrintLn(IA_T_ATOM *pAtom) {
  iaPrint(pAtom);
  printf("\n");
}

void *iaGetStackDataPtr(IA_T_ATOM *pAtom) {
  return &(pAtom->data.c);
}

void *iaGetHeapDataPtr(IA_T_ATOM *pAtom) {
  return &(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]);
}

void *iaGetDataPtr(IA_T_ATOM *pAtom) {
  if (pAtom->onHeap) {
    return iaGetHeapDataPtr(pAtom);
  } else {
    return iaGetStackDataPtr(pAtom);
  }
}

void *iaGetIndexPtr(IA_T_ATOM *pAtom, size_t index) {
  uint8_t *pData = (uint8_t *)iaGetDataPtr(pAtom);
  return &(pData[index*iaGetRecsize(pAtom)]);
}

void *iaGetIndexPtr2D(IA_T_ATOM *patom, size_t index1, size_t index2, bool verbose) {
  if (patom->type != IA_ID_ATOM) {
    if (verbose) printf("ERR: iaGetIndexPtr2D called on non-atom\n");
    return NULL;
  }
  if (patom->count <= index1) {
    if (verbose) printf("ERR: iaGetIndexPtr2D index1 out of bounds\n");
    return NULL;
  }
  IA_T_ATOM *pA = (IA_T_ATOM *)iaGetIndexPtr(patom, index1);
  if (pA->count <= index2) {
    if (verbose) printf("ERR: iaGetIndexPtr2D index2 out of bounds\n");
    return NULL;
  }
  return iaGetIndexPtr(pA, index2);
}

void *iaGetIndexPtr3D(IA_T_ATOM *patom, size_t index1, size_t index2, size_t index3, bool verbose) {
  if (!patom || patom->type != IA_ID_ATOM) {
    if (verbose)printf("ERR: iaGetIndexPtr3D called on non-atom\n");
    return NULL;
  }
  if (patom->count <= index1) {
    if (verbose) printf("ERR: iaGetIndexPtr3D index1 out of bounds\n");
    return NULL;
  }
  IA_T_ATOM *pA = (IA_T_ATOM *)iaGetIndexPtr(patom, index1);
  if (!pA || pA->type != IA_ID_ATOM || pA->count <= index2) {
    if (verbose)printf("ERR: iaGetIndexPtr3D index2 out of bounds or not dim=3\n");
    return NULL;
  }
  IA_T_ATOM *pB = (IA_T_ATOM *)iaGetIndexPtr(pA, index2);
  if (!pB || pB->count <= index3) {
    if (verbose) printf("ERR: iaGetIndexPtr3D index3 out of bounds\n");
    return NULL;
  }
  return iaGetIndexPtr(pB, index3);
}

bool iaExpand(IA_T_ATOM *pAtom, size_t new_capacity) {
  if (pAtom->onHeap == 0) {
    size_t recsize = iaGetRecsize(pAtom);
    if (new_capacity <= iaStackMax[pAtom->type]) {
      return true;
    } else {
      // new capacity is next larger power of 2 of new_capacity:
      new_capacity = _getNextLargestPowerOf2(new_capacity);
      if (new_capacity < IA_MIN_NEW_EXPAND) {
        new_capacity = IA_MIN_NEW_EXPAND;
      }
      IA_T_ATOM oldAtom;
      memcpy(&oldAtom, pAtom, sizeof(IA_T_ATOM));
      size_t alloc_size = sizeof(IA_T_HEAP_HEADER)+recsize*new_capacity;
      pAtom->data.pHeap = (void *)malloc(alloc_size);
      if (pAtom->data.pHeap == NULL) {
        return false;
      }
      memset(pAtom->data.pHeap, 0, alloc_size);
      pAtom ->type = oldAtom.type;
      pAtom->onHeap = 1;
      pAtom->count = oldAtom.count;
      pAtom->data.pHeap->capacity = new_capacity;
      pAtom->data.pHeap->recsize = recsize;
      void *pdest = iaGetHeapDataPtr(pAtom);
      void *psrc = iaGetStackDataPtr(&oldAtom);
      memcpy(pdest, psrc, recsize*pAtom->count);
      return true;
    }
  } else {
    if (new_capacity <= pAtom->data.pHeap->capacity) {
      return true;
    }
    size_t recsize = pAtom->data.pHeap->recsize;
    if (recsize == 0) {
      printf("ERR: recsize is 0 at expand!\n");
      return false;
    }
    size_t act_new_capacity = _getNextLargestPowerOf2(new_capacity);
    if (act_new_capacity < IA_MIN_NEW_EXPAND) {
      act_new_capacity = IA_MIN_NEW_EXPAND;
    }
    if (act_new_capacity <= pAtom->data.pHeap->capacity) {
      printf("Calculation of act new capacity went bad: old: %ld new: %ld\n", pAtom->data.pHeap->capacity, act_new_capacity);
      return false;
    }
    size_t old_size = sizeof(IA_T_HEAP_HEADER)+recsize*pAtom->count;
    size_t new_size = sizeof(IA_T_HEAP_HEADER)+recsize*act_new_capacity;
    void *p = realloc(pAtom->data.pHeap, new_size);
    if (p == NULL) {
      return false;
    }
    memset((uint8_t *)p+old_size, 0, new_size-old_size);
    pAtom->data.pHeap = p;
    pAtom->data.pHeap->capacity = act_new_capacity;
    if (pAtom->data.pHeap->recsize != recsize) printf("ERR: recsize mismatch after realloc!\n");
    return true;
  }
}

bool iaSetIndex(IA_T_ATOM *pAtom, size_t index, void *pData) {
  if (index >= pAtom->count) {
    size_t recsize = iaGetRecsize(pAtom);
    if (pAtom->onHeap == 0) {
      if (index >= iaStackMax[pAtom->type]) {
        return false;
      }
    } else {
      if (index >= pAtom->data.pHeap->capacity) {
        return false;
      }
    }
  }
  size_t recsize = iaGetRecsize(pAtom);
  void *pdest = iaGetIndexPtr(pAtom, index);
  if (pAtom->type != IA_ID_ATOM) {
    memcpy(pdest, pData, recsize);
  } else {
    IA_T_ATOM *pA = (IA_T_ATOM *)pdest;
    _recCopy(pA, pData);
  }
  if (index+1 > pAtom->count) {
    pAtom->count = index+1;
  }
  return true;
}

bool iaSetIndexExpand(IA_T_ATOM *pAtom, size_t index, void *pData) {
  bool expanded = false;
  if (index >= pAtom->count) {
    if (pAtom->onHeap == 0) {
      if (index >= iaStackMax[pAtom->type]) {
        if (!iaExpand(pAtom, index+1)) return false;
        expanded = true;
      }
    } else {
      if (index >= pAtom->data.pHeap->capacity) {
        if (!iaExpand(pAtom, index+1)) return false;
        expanded = true;
      }
    }
  }
  // if (!expanded) printf("SetIndexExpand didn't require expand\n");
  size_t recsize = iaGetRecsize(pAtom);
  if (recsize == 0) {
    printf("ERR: recsize is 0 at setIndexExpand!\n");
    return false;
  }
  void *pdest = iaGetIndexPtr(pAtom, index);
  if (pdest == NULL) {
    printf("Error getting index %ld\n", index);
    return false;
  }
  if (pAtom->type != IA_ID_ATOM) {
    memcpy(pdest, pData, recsize);
  } else {
    IA_T_ATOM *pA = (IA_T_ATOM *)pdest;
    _recCopy(pA, pData);
  }
  if (index+1 > pAtom->count) {
    pAtom->count = index+1;
  }
  return true;
}

bool iaAppend(IA_T_ATOM *pAtom, void *pData) {
  if (pAtom->onHeap) {
    if (pAtom->data.pHeap->recsize == 0) {
      printf("ERR: recsize is 0 at append!\n");
      return false;
    }
  }
  bool ret = iaSetIndexExpand(pAtom, pAtom->count, pData);
  return ret;
}

bool iaCopy(IA_T_ATOM *pSrc, IA_T_ATOM *pDest) {
  if (!pSrc->onHeap) {
    *pDest = *pSrc;
    return true;
  } else {
    return iaCreate(pDest, pSrc->type, iaGetRecsize(pSrc), pSrc->count, iaGetDataPtr(pSrc));
  }
}

bool iaJoin(IA_T_ATOM *pAtom, IA_T_ATOM *pAppend) {
  size_t new_size = pAtom->count + pAppend->count;
  if (!iaExpand(pAtom, new_size)) {
    return false;
  }
  uint8_t *pdest = iaGetIndexPtr(pAtom, pAtom->count);
  uint8_t *psrc = iaGetDataPtr(pAppend);
  size_t recsize = iaGetRecsize(pAppend);
  if (pAtom->type != IA_ID_ATOM) {
    memcpy(pdest, psrc, recsize*pAppend->count);
  pAtom->count = new_size;
  } else {
    IA_T_ATOM *pA = (IA_T_ATOM *)pdest;
    _recCopy(pA, pAppend);
    pAtom->count += 1;
  }
  return true;
}

bool iaSlice(IA_T_ATOM *pSrc, IA_T_ATOM *pDest, size_t start, size_t len) {
  
  if (start >= pSrc->count) {
    printf("BAD SLICE\n");
    if (!iaCreate(pDest, pSrc->type, pSrc->type, 0, NULL)) {
      return false;
    }
    return true;
  }
  if (start+len > pSrc->count) {
    len = pSrc->count-start;
  }
  if (!iaCreate(pDest, pSrc->type, iaGetRecsize(pSrc), len, iaGetIndexPtr(pSrc, start))) {
    return false;
  }
  return true;
}

// ----------------- MAP -------------------
bool iaCreateMap(IA_T_MAP *pMap, size_t capacity) {
  capacity = _getNextLargestPowerOf2(capacity);
  iaCreateCapacity(&(pMap->hash), IA_ID_ATOM, sizeof(IA_T_ATOM), capacity, 0, NULL);
  iaCreateCapacity(&(pMap->values), IA_ID_ATOM, sizeof(IA_T_ATOM), capacity, 0, NULL);
  pMap->hash.count = capacity;
  pMap->values.count = capacity;
  pMap->fillLevel = 0;
  return true;
}

bool iaMapGet(IA_T_MAP *pMap, IA_T_ATOM *pKey, IA_T_ATOM *pValue) {
  void *pKeyData = iaGetDataPtr(pKey);
  unsigned long recsize = iaGetRecsize(pKey);
  unsigned long hash = iaCrc32(pKeyData, recsize*pKey->count) % pMap->hash.data.pHeap->capacity;
  //printf("Get-Hash: %ld, count: %ld\n", hash, pMap->hash.count);
  if (hash > pMap->hash.count) {
    return false;
  }
  IA_T_ATOM *pKeyEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->hash), hash);
  IA_T_ATOM *pValueEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->values), hash);
  if (pKeyEntry->count == 0) {
    return false;
  }
  if (pKeyEntry->count != pValueEntry->count) {
    printf("value and key tables in map have different counts\n");
    return false;
  }
  for (unsigned long i=0; i<pKeyEntry->count; i++) {
    IA_T_ATOM *pKey2 = (IA_T_ATOM *)iaGetIndexPtr(pKeyEntry, i);
    if (pKey2->type != pKey->type) {
      continue;
    }
    unsigned long entry_size = iaGetRecsize(pKey2) * pKey2->count;
    if (entry_size != recsize*pKey->count) {
      continue;
    }
    if (!memcmp(iaGetDataPtr(pKey2), pKeyData, recsize*pKey->count)) {
      IA_T_ATOM *pValue2 = (IA_T_ATOM *)iaGetIndexPtr(pValueEntry, i);
      iaCreate(pValue, pValue2->type, iaGetRecsize(pValue2), pValue2->count, iaGetDataPtr(pValue2));
      return true;
    }
  }
  return false;  
}

bool iaMapSet(IA_T_MAP *pMap, IA_T_ATOM *pKey, IA_T_ATOM *pValue) {
  void *pKeyData = iaGetDataPtr(pKey);
  unsigned long recsize = iaGetRecsize(pKey);
  unsigned long hash = iaCrc32(pKeyData, recsize*pKey->count) % (pMap)->hash.data.pHeap->capacity;
  if (pMap->hash.count * 3 < pMap->fillLevel *4) {
    //printf("Rehashing\n");
    IA_T_MAP copyMap;
    iaCreateMap(&copyMap, pMap->hash.data.pHeap->capacity*2);
    for (unsigned long i=0; i<pMap->hash.count; i++) {
      IA_T_ATOM *pKeyEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->hash), i);
      IA_T_ATOM *pValueEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->values), i);
      for (unsigned long j=0; j<pKeyEntry->count; j++) {
        IA_T_ATOM *pKey2 = (IA_T_ATOM *)iaGetIndexPtr(pKeyEntry, j);
        IA_T_ATOM *pValue2 = (IA_T_ATOM *)iaGetIndexPtr(pValueEntry, j);
        iaMapSet(&copyMap, pKey2, pValue2);
      }
    }
    iaMapDelete(pMap);
    *pMap = copyMap;
  }
  IA_T_ATOM *pKeyEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->hash), hash);
  IA_T_ATOM *pValueEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->values), hash);
  if (pKeyEntry->count == 0) {
    if (!iaCreate(pKeyEntry, IA_ID_ATOM, sizeof(IA_T_ATOM), 1, pKey)) {
      return false;
    }
    if (!iaCreate(pValueEntry, IA_ID_ATOM, sizeof(IA_T_ATOM), 1, pValue)) {
      return false;
    }
    pMap->fillLevel += 1;
    return true;
  }
  for (unsigned long i=0; i<pKeyEntry->count; i++) {
    IA_T_ATOM *pKey2 = (IA_T_ATOM *)iaGetIndexPtr(pKeyEntry, i);
    if (pKey2->type != pKey->type) {
      continue;
    }
    unsigned long entry_size = iaGetRecsize(pKey2) * pKey2->count;
    if (entry_size != recsize*pKey->count) {
      continue;
    }
    if (!memcmp(iaGetDataPtr(pKey2), pKeyData, recsize*pKey->count)) {
      IA_T_ATOM *pValue2 = (IA_T_ATOM *)iaGetIndexPtr(pValueEntry, i);
      iaDelete(pValue2);
      if (!iaCreate(pValue2, pValue->type, iaGetRecsize(pValue), pValue->count, iaGetDataPtr(pValue))) {
        return false;
      }
      return true;
    }
  }
  if (!iaAppend(pKeyEntry, pKey)) {
    return false;
  }
  if (!iaAppend(pValueEntry, pValue)) {
    return false;
  }
  pMap->fillLevel += 1;
  return true;
}

void iaMapDelete(IA_T_MAP *pMap) {
  iaDelete(&(pMap->hash));
  iaDelete(&(pMap->values));
}

bool iaMapRemove(IA_T_MAP *pMap, IA_T_ATOM *pKey) {
  void *pKeyData = iaGetDataPtr(pKey);
  unsigned long recsize = iaGetRecsize(pKey);
  unsigned long hash = iaCrc32(pKeyData, recsize*pKey->count) % pMap->hash.data.pHeap->capacity;
  if (hash > pMap->hash.count) {
    return false;
  }
  IA_T_ATOM *pKeyEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->hash), hash);
  IA_T_ATOM *pValueEntry = (IA_T_ATOM *)iaGetIndexPtr(&(pMap->values), hash);
  if (pKeyEntry->count == 0) {
    return false;
  }
  for (unsigned long i=0; i<pKeyEntry->count; i++) {
    IA_T_ATOM *pKey2 = (IA_T_ATOM *)iaGetIndexPtr(pKeyEntry, i);
    if (pKey2->type != pKey->type) {
      continue;
    }
    unsigned long entry_size = iaGetRecsize(pKey2) * pKey2->count;
    if (entry_size != recsize*pKey->count) {
      continue;
    }
    if (!memcmp(iaGetDataPtr(pKey2), pKeyData, recsize*pKey->count)) {
      iaDelete(pKeyEntry);
      iaDelete(pValueEntry);
      for (unsigned long j=i+1; j<pKeyEntry->count; j++) {
        pKey2 = (IA_T_ATOM *)iaGetIndexPtr(pKeyEntry, j);
        IA_T_ATOM *pValue = (IA_T_ATOM *)iaGetIndexPtr(pValueEntry, hash);
        iaSetIndex(pKeyEntry, j-1, pKey2);
        iaSetIndex(pValueEntry, j-1, pValue);
        iaDelete(pKey2);
        iaDelete(pValue);
      }
      pKeyEntry->count -= 1;
      pValueEntry->count -= 1;
      pMap->fillLevel -= 1;
      return true;
    }
  }
  return false;
}

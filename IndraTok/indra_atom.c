#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "indra_atom.h"

const unsigned long iaStackMax[] = {0, IA_STACK_CHARS, IA_STACK_WORDS, IA_STACK_INTS, IA_STACK_LONGS, IA_STACK_FLOATS, IA_STACK_DOUBLES, 0, 0};
const unsigned long iaTypesize[IA_ID_PANY+1] = {0, sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t), sizeof(float), sizeof(double), sizeof(struct _ia_atom), sizeof(void *)};

void iaDelete(IA_T_ATOM *pAtom) {
  if (pAtom->type == IA_ID_PANY) {
    free(pAtom->data.pHeap);
  } else if (pAtom->type == IA_ID_ATOM) {
    for (uint64_t i=0; i<pAtom->count; i++) {
      IA_T_ATOM *pA = (IA_T_ATOM *)iaGetIndexPtr(pAtom, i);
      iaDelete(pA);
    }
    free(pAtom->data.pHeap);
  } else {
    if (pAtom->onHeap) {
      free(pAtom->data.pHeap);
    }
  }
}

bool iaCopyData(uint8_t *pDest, uint8_t *pSrc, IA_T_TYPES type, size_t recsize, size_t count) {
  for (size_t i=0; i<count; i++) {
    memcpy(pDest, pSrc, recsize);
    // printf("%ld/%ld Copied %ld bytes\n", i+1, count, recsize);
    if (type == IA_ID_ATOM) {
      IA_T_ATOM *pSrcT = (IA_T_ATOM *)pSrc;
      IA_T_ATOM *pDestT = (IA_T_ATOM *)pDest;
      void *pSrcData = iaGetDataPtr(pSrcT);
      void *pDestData = iaGetDataPtr(pDestT);
      if (((IA_T_ATOM *)pSrc)->onHeap) {
        if (!iaCopyData(pDestData, pSrcData, pSrcT->type, iaTypesize[pSrcT->type], pSrcT->count)) {
          return false;
        }
      }
    }
    pDest += recsize;
    pSrc += recsize;
  }
  return true;
}

bool iaCreate(IA_T_ATOM *pAtom, int type, size_t recsize, size_t count, void *pData) {
  pAtom->onHeap = 0;
  pAtom->type = type;
  if (type == IA_ID_NIL) {
    pAtom->count = 0;
    return true;
  }
  pAtom->count = count;
  if (count > iaStackMax[type]) {
    pAtom->data.pHeap = (IA_T_HEAP_HEADER *)malloc(sizeof(IA_T_HEAP_HEADER)+recsize*count);
    if (pAtom->data.pHeap == NULL) {
      return false;
    }
    pAtom->onHeap = 1;
    pAtom->data.pHeap->capacity = count;
    pAtom->data.pHeap->recsize = recsize;
    if (type == IA_ID_ATOM) {
      IA_T_ATOM *pSrc = pData;
      if (pSrc->onHeap) {
        if (!iaCopyData(iaGetHeapDataPtr(pAtom), iaGetHeapDataPtr(pSrc), pSrc->type, iaTypesize[pSrc->type], pSrc->count)) {
          return false;
        }
      } else {
        void *pdest = iaGetDataPtr(pAtom);
        if (count*recsize>0) memcpy(pdest, pData, recsize*count);
      }
    } else {
      void *pdest = iaGetDataPtr(pAtom);
      if (count*recsize>0) memcpy(pdest, pData, recsize*count);
    }
  } else {
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
  
void iaPrint(IA_T_ATOM *pAtom) {
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
  if (pAtom->count > 1) {
    printf("[");
  }
  for (size_t i=0; i<pAtom->count; i++) {
    void *pData = iaGetIndexPtr(pAtom, i);
    if (!pData) {
      printf("Error getting index %ld\n", i);
      return;
    }
    switch (pAtom->type) {
    case IA_ID_CHAR:
      printf("%c", *(uint8_t *)pData);
      break;
    case IA_ID_WORD:
      printf("%d", *(uint16_t *)pData);
      break;
    case IA_ID_INT:
      printf("%d", *(uint32_t *)pData);
      break;
    case IA_ID_LONG:
      printf("%ld", *(uint64_t *)pData);
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
      iaPrint(pData);
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
  return &(pData[index*iaTypesize[pAtom->type]]);
}

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

bool iaExpand(IA_T_ATOM *pAtom, size_t new_capacity) {
  if (pAtom->onHeap == 0) {
    size_t recsize = iaTypesize[pAtom->type];
    if (new_capacity <= iaStackMax[pAtom->type]) {
      return true;
    } else {
      // new capacity is next larger power of 2 of new_capacity:
      new_capacity = _getNextLargestPowerOf2(new_capacity);
      if (new_capacity < 8) {
        new_capacity = 8;
      }
      IA_T_ATOM OldAtom;
      memcpy(&OldAtom, pAtom, sizeof(IA_T_ATOM));
      size_t alloc_size = sizeof(IA_T_HEAP_HEADER)+recsize*new_capacity;
      pAtom->data.pHeap = (void *)malloc(alloc_size);
      if (pAtom->data.pHeap == NULL) {
        return false;
      }
      // printf("Expanded from stack to heap, new heap-capacity: %ld -> %ld, allocated %ld bytes\n", iaStackMax[pAtom->type], new_capacity, alloc_size);
      pAtom->onHeap = 1;
      pAtom->data.pHeap->capacity = new_capacity;
      pAtom->data.pHeap->recsize = recsize;
      // printf("Moving %ld bytes from stack to heap\n", recsize*pAtom->count);
      void *pdest = iaGetHeapDataPtr(pAtom);
      void *psrc = iaGetStackDataPtr(&OldAtom);
      memcpy(pdest, psrc, recsize*pAtom->count);
      return true;
    }
  } else {
    if (new_capacity <= pAtom->data.pHeap->capacity) {
      return true;
    }
    size_t recsize = pAtom->data.pHeap->recsize;
    if (new_capacity >= pAtom->data.pHeap->capacity) {
      size_t act_new_capacity = _getNextLargestPowerOf2(new_capacity);
      size_t new_size = sizeof(IA_T_HEAP_HEADER)+recsize*act_new_capacity;
      void *p = realloc(pAtom->data.pHeap, new_size);
      if (p == NULL) {
        return false;
      }
      pAtom->data.pHeap = p;
      pAtom->data.pHeap->capacity = act_new_capacity;
      // printf("Expanded heap-capacity: %ld recsize: %ld\n", pAtom->data.pHeap->capacity, pAtom->data.pHeap->recsize);
      return true;
    } else {
      printf("Shrinking not supported\n");
      return false;  // no shrinking
    }    
  }
}

bool iaSetIndex(IA_T_ATOM *pAtom, size_t index, void *pData) {
  if (index >= pAtom->count) {
    size_t recsize = iaTypesize[pAtom->type];
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
  size_t recsize = iaTypesize[pAtom->type];
  void *pdest = iaGetIndexPtr(pAtom, index);
  iaCopyData(pdest, pData, pAtom->type, recsize, 1);
  if (index+1 > pAtom->count) {
    pAtom->count = index+1;
  }
  return true;
}

bool iaSetIndexExpand(IA_T_ATOM *pAtom, size_t index, void *pData) {
  if (index >= pAtom->count) {
    if (pAtom->onHeap == 0) {
      if (index >= iaStackMax[pAtom->type]) {
        // printf("SetExpand requires expand (currently stack)\n");
        if (!iaExpand(pAtom, index+1)) return false;
      }
    } else {
      if (index >= pAtom->data.pHeap->capacity) {
        // printf("SetExpand requires expand (currently heap)\n");
        if (!iaExpand(pAtom, index+1)) return false;
      }
    }
  }
  size_t recsize = iaTypesize[pAtom->type];
  void *pdest = iaGetIndexPtr(pAtom, index);
  if (pdest == NULL) {
    // printf("Error getting index %ld\n", index);
    return false;
  }
  iaCopyData(pdest, pData, pAtom->type, recsize, 1);
  if (index+1 > pAtom->count) {
    // printf("Setting count to %ld\n", index+1);
    pAtom->count = index+1;
  }
  return true;
}

bool iaAppend(IA_T_ATOM *pAtom, void *pData) {
  return iaSetIndexExpand(pAtom, pAtom->count, pData);
}

bool iaCopy(IA_T_ATOM *pSrc, IA_T_ATOM *pDest) {
  if (pSrc->onHeap) {
    *pDest = *pSrc;
    return true;
  } else {
    return iaCreate(pDest, pSrc->type, iaTypesize[pSrc->type], pSrc->count, iaGetDataPtr(pSrc));
  }
}

bool iaJoin(IA_T_ATOM *pAtom, IA_T_ATOM *pAppend) {
  size_t new_size = pAtom->count + pAppend->count;
  if (!iaExpand(pAtom, new_size)) {
    return false;
  }
  uint8_t *pdest = iaGetIndexPtr(pAtom, pAtom->count);
  uint8_t *psrc = iaGetDataPtr(pAppend);
  size_t recsize = iaTypesize[pAtom->type];
  if (!iaCopyData(pdest, psrc, pAtom->type, recsize, pAppend->count)) {
    return false;
  }
  pAtom->count = new_size;
  return true;
}

bool iaSlice(IA_T_ATOM *pSrc, IA_T_ATOM *pDest, size_t start, size_t len) {
  if (start >= pSrc->count) {
    return false;
  }
  if (start+len > pSrc->count) {
    len = pSrc->count-start;
  }
  if (!iaCreate(pDest, pSrc->type, iaTypesize[pSrc->type], len, iaGetIndexPtr(pSrc, start))) {
    return false;
  }
  return true;
}

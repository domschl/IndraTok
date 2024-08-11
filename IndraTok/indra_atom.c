#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "indra_atom.h"

void iaDelete(IA_T_ATOM *pAtom) {
  if (pAtom->type == IA_ID_PANY) {
    free(pAtom->data.pHeap);
  } else if (pAtom->type == IA_ID_ATOM) {
    for (uint64_t i=0; i<pAtom->count; i++) {
      iaDelete(&(((IA_T_ATOM *)(pAtom->data.pHeap + sizeof(IA_T_HEAP_HEADER)))[i]));
    }
    free(pAtom->data.pHeap);
  } else {
    if (pAtom->onHeap) {
      free(pAtom->data.pHeap);
    }
  }
}

bool iaCreate(IA_T_ATOM *pAtom, int type, size_t recsize, size_t count, void *pData) {
  pAtom->onHeap = 0;
  pAtom->type = type;
  if (type == IA_ID_NIL) {
    pAtom->count = 0;
    return true;
  }
  pAtom->count = count;
  printf("Setting %ld elements of type %d, stackMax: %ld\n", count, type, iaStackMax[type]);
  if (count > iaStackMax[type]) {
    printf("Allocating %ld bytes\n", (long)sizeof(IA_T_HEAP_HEADER)+recsize*count);
    pAtom->data.pHeap = (IA_T_HEAP_HEADER *)malloc(sizeof(IA_T_HEAP_HEADER)+recsize*count);
    if (pAtom->data.pHeap == NULL) {
      return false;
    }
    pAtom->onHeap = 1;
    pAtom->data.pHeap->capacity = count;
    pAtom->data.pHeap->recsize = recsize;
    printf("Copying %ld bytes\n", recsize*count);
    memcpy((pAtom->data.pHeap + sizeof(IA_T_HEAP_HEADER)), pData, recsize*count);
  } else {
    printf("Using stack for %ld bytes\n", recsize*count);
    memcpy(pAtom->data.c, pData, recsize*count);
  }
  return true;
}


void iaSetChar(IA_T_ATOM *pAtom, uint8_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_CHAR;
  pAtom->count = 1;
  pAtom->data.c[0] = value;
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
  
bool iaSetString(IA_T_ATOM *pAtom, char *pString) {
  return iaCreate(pAtom, IA_ID_CHAR, sizeof(uint8_t), strlen(pString)+1, pString);
}

void *iaGetDataPtr(IA_T_ATOM *pAtom) {
  if (pAtom->type == IA_ID_PANY) {
    return (void *)(pAtom->data.pHeap + sizeof(IA_T_HEAP_HEADER));
  } else if (pAtom->type == IA_ID_ATOM) {
    return pAtom->data.pHeap;
  } else {
    if (pAtom->onHeap) {
      return (void *)(pAtom->data.pHeap + sizeof(IA_T_HEAP_HEADER));
    } else {
      return &pAtom->data;
    }
  }
}

void iaPrint(IA_T_ATOM *pAtom) {
  if (pAtom->count > 1) {
    printf("[");
  }
  for (size_t i=0; i<pAtom->count; i++) {
    void *pData;
    if (!iaGetIndex(pAtom, &pData, i)) {
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
        printf("[");
        for (uint64_t i=0; i<pAtom->count; i++) {
          iaPrint(&(((IA_T_ATOM *)(pAtom->data.pHeap + sizeof(IA_T_HEAP_HEADER)))[i]));
        }
        printf("]");
      break;
    default:
      printf("Unknown type %d\n", pAtom->type);
    }
  }
  if (pAtom->count > 1) {
    printf("]");
  }
}

void *iaGetIndexPtr(IA_T_ATOM *pAtom, size_t index) {
  if (index >= pAtom->count) {
    return NULL;
  }
  if (pAtom->type == IA_ID_PANY) {
    return &((&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]))[index*pAtom->data.pHeap->recsize]);
  } else if (pAtom->type == IA_ID_ATOM) {
    return (void *)&(((IA_T_ATOM *)&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]))[index]);
  } else {
    if (pAtom->onHeap) {
      return &(((uint8_t *)&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]))[index*pAtom->data.pHeap->recsize]);
    } else {
      return &(((uint8_t *)&(pAtom->data.c))[index*iaTypesize[pAtom->type]]);
    }
  }
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
      printf("Expanded from stack to heap, new heap-capacity: %ld -> %ld, allocated %ld bytes\n", iaStackMax[pAtom->type], new_capacity, alloc_size);
      pAtom->onHeap = 1;
      pAtom->data.pHeap->capacity = new_capacity;
      pAtom->data.pHeap->recsize = recsize;
      printf("Moving %ld bytes from stack to heap\n", recsize*pAtom->count);
      memcpy(&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]), &(OldAtom.data.c), recsize*pAtom->count);
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
      //void *p = malloc(new_size);
      if (p == NULL) {
        return false;
      }
      //memcpy(p, pAtom->data.pHeap, sizeof(IA_T_HEAP_HEADER)+recsize*pAtom->count);
      //free(pAtom->data.pHeap);
      pAtom->data.pHeap = p;
      pAtom->data.pHeap->capacity = act_new_capacity;
      printf("Expanded heap-capacity: %ld recsize: %ld\n", pAtom->data.pHeap->capacity, pAtom->data.pHeap->recsize);
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
  if (pAtom->onHeap == 0) {
    memcpy(&(((uint8_t *)&(pAtom->data.c))[index*recsize]), pData, recsize);
  } else {
    memcpy(&((&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)]))[index*recsize]), pData, recsize);
  }
  if (index+1 > pAtom->count) {
    pAtom->count = index+1;
  }
  return true;
}

bool iaSetIndexExpand(IA_T_ATOM *pAtom, size_t index, void *pData) {
  if (index >= pAtom->count) {
    if (pAtom->onHeap == 0) {
      if (index >= iaStackMax[pAtom->type]) {
        printf("SetExpand requires expand (currently stack)\n");
        if (!iaExpand(pAtom, index+1)) return false;
      }
    } else {
      if (index >= pAtom->data.pHeap->capacity) {
        printf("SetExpand requires expand (currently heap)\n");
        if (!iaExpand(pAtom, index+1)) return false;
      }
    }
  }
  size_t recsize = iaTypesize[pAtom->type];
  if (pAtom->onHeap == 0) {
    memcpy(&(((uint8_t *)&(pAtom->data.c))[index*recsize]), pData, recsize);
  } else {
    memcpy(&(((uint8_t *)(&(((uint8_t *)pAtom->data.pHeap)[sizeof(IA_T_HEAP_HEADER)])))[index*recsize]), pData, recsize);
  }
  if (index+1 > pAtom->count) {
    printf("Setting count to %ld\n", index+1);
    pAtom->count = index+1;
  }
  return true;
}

bool iaAppend(IA_T_ATOM *pAtom, void *pData) {
  return iaSetIndexExpand(pAtom, pAtom->count, pData);
}

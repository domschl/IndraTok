#include <stdlib.h>
#include <string.h>
#include "indra_atom.h"

void iaDelete(IA_T_ATOM *pAtom) {
  if (pAtom->type == IA_ID_PANY) {
    free(pAtom->data.pHeap);
  } else if (pAtom->type == IA_ID_ATOM) {
    for (uint64_t i=0; i<pAtom->count; i++) {
      iaDelete(&(((IA_T_ATOM *)(pAtom->data.pHeap->pData))[i]));
    }
    free(pAtom->data.pHeap);
  } else {
    if (pAtom->onHeap) {
      free(pAtom->data.pHeap);
    }
  }
}

void iaSet(IA_T_ATOM *pAtom, int type, uint64_t recsize, uint64_t count, void *pData) {
  pAtom->onHeap = 0;
  pAtom->type = type;
  if (type == IA_ID_NIL) {
    pAtom->count = 0;
    return;
  }
  pAtom->count = count;
  if (count > iaStackMax[type]) {
    pAtom->onHeap = 1;
    pAtom->data.pHeap = (IA_T_HEAP *)malloc(sizeof(IA_T_HEAP_HEADER)+recsize*count);
    pAtom->data.pHeap->capacity = count;
    pAtom->data.pHeap->recsize = recsize;
    memcpy(pAtom->data.pHeap->pData, pData, recsize*count);
  } else {
    memcpy(pAtom->data.c, pData, recsize*count);
  }
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
  

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "indra_atom.h"

void iaDelete(IA_T_ATOM *pAtom) {
  if (pAtom->onHeap) {
    if (pAtom->type == IA_ID_ARRAY) {
      free(pAtom->dat.heap_atom->pData);
    } else if (pAtom->type == IA_ID_ATOM) {
      for (uint64_t i=0; i<pAtom->count; i++) {
        if (pAtom->dat.heap_atom[i].type == IA_ID_ARRAY) {
          free(pAtom->dat.heap_atom[i].pData);
        } else if (pAtom->dat.heap_atom[i].type == IA_ID_ATOM) {
          iaDelete((IA_T_ATOM *)pAtom->dat.heap_atom[i].pData);
        }
      }
    } else if (pAtom->type == IA_ID_ANYPTR) {
      for (uint64_t i=0; i<pAtom->count; i++) {
        free(pAtom->dat.anyPtr[i]);
      }
    } else {
      printf("Unexpected type %d on heap, FATAL\n", pAtom->type);
      exit(-1);
    }      
  }
}



void iaSetChar(IA_T_ATOM *pAtom, uint8_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_CHAR;
  pAtom->count = 1;
  pAtom->dat.c[0] = value;
}

void iaSetWord(IA_T_ATOM *pAtom, uint16_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_CHAR;
  pAtom->count = 1;
  pAtom->dat.w[0] = value;
}

void iaSetInt(IA_T_ATOM *pAtom, uint32_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_INT;
  pAtom->count = 1;
  pAtom->dat.i[0] = value;
}

void iaSetLong(IA_T_ATOM *pAtom, uint64_t value) {
  pAtom->onHeap = 0;
  pAtom->type = IA_ID_LONG;
  pAtom->count = 1;
  pAtom->dat.l[0] = value;
}

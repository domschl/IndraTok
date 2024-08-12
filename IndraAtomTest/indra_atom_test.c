#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define IA_STACK_PREFERENCE 4

#include "indra_atom.h"

int main() {
  IA_T_ATOM atom;
  /*
  char buf[101];
  for (int i=1; i<20; i++) {
    memset(buf, 0, 101);
    
    for (int j=0; j<i; j++) {
      buf[j] = (char)(j%26)+'A';
    }
    buf[i] = 0;
    iaSetString(&atom, buf);
    char *p = (char *)iaGetDataPtr(&atom);
    // if (atom.onHeap) {
    //   printf("%03d Heap:  %03ld, >%s<\n", i, strlen(p), p);
    // } else {
    //    printf("%03d Stack: %03ld, >%s<\n", i, strlen(p), p);
    // }
    iaDelete(&atom);
  }
  printf("----------------\n");
  */
  iaSetString(&atom, "");
  atom.count = 0;
  for (int i=0; i<20; i++) {
    char c = ('A'+(i%26));
    iaSetIndexExpand(&atom, i, &c);
    if (atom.onHeap) {
      printf("Heap: [%ld] ", atom.count);
    } else {
      printf("Stack: [%ld] ", atom.count);
    }
    iaPrint(&atom);
    printf("\n");
  }
  if (atom.onHeap) {
    free(atom.data.pHeap);
  }
  return 0;
}

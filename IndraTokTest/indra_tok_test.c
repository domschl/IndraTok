// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_ent.h"
#include "indra_tok.h"
#include "crc_crypt_hash.h"

bool makeString(IndraEnt **ppa, const char *str) {
  //printf("Creating string: %s\n", str);
  (*ppa) = itCreateString(str);
  if (!stringValidateUtf8(*ppa)) {
    printf("Bad string: <%s>\n", str);
    return false;
  }
  return true;
}

typedef struct _char_conv_test {
  char *charString;
  unsigned long utf8Len;
} CharConvTest;

CharConvTest test1[] = {{ "Smörö", 5}, {"རྒྱུད་", 6}, {"", 0}, {"𐍈", 1}, {"😁", 1}};

int main(int argc, char *argv[]) {
  unsigned int errs = 0, oks=0;
  IndraEnt *a, *b, *c;
  if (!makeString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    errs += 1;
  } else {
    oks += 1;
  }
  // printf("String: "); stringPrintLn(&a); printf(" | Part 5,3: ");
  stringPartBytes(a, &b, 5, 3);
  // stringPrintLn(&b);
  char *pStr = (char *)b->buf;
  if (strcmp(", w", pStr)) {
    printf("ERROR: part expected to be >, w<, got: >%s<.\n", pStr);
    errs += 1;
  } else {
    oks += 1;
  }
  itDelete(a);
  
  a = itCreateString("Hello, ");
  b = itCreateString("world!");
  stringAppend(a, b);
  itPrintLn(a);
  pStr = (char *)a->buf;
  if (strcmp("Hello, world!", pStr)) {
    errs += 1;
    printf("Expected >Hello, world!<, got: >%s<\n", pStr);
  } else {
    oks += 1;
  }
  itDelete(a); itDelete(b);
  
  c = itCreateString("");
  unsigned long sum=0;
  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    a = itCreateString(test1[i].charString);
    pStr = (char *)a->buf;
    if (strcmp(pStr, test1[i].charString)) {
      printf("ERROR: Conversion cycle failed for >%s<, result >%s<\n", test1[i].charString, pStr);
      errs += 1;
    } else {
      oks += 1;
    }
    stringAppend(c, a); sum+=test1[i].utf8Len;
    // stringDisplayHex(&a);
    unsigned long len=stringLenUtf8(a);
    if (len != test1[i].utf8Len) {
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", test1[i].charString, len, test1[i].utf8Len); 
      errs += 1;
    } else {
      oks += 1;
    }
    len = stringLenUtf8(c);
    if (len != sum) {
      pStr = (char *)c->buf;
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", pStr, len, sum);
      errs += 1;
    } else {
      oks += 1;
    }
    itDelete(a);
  }
  itDelete(c);

  a = itCreateString("Hello, World!");
  b = itCreateString("orld!");
  long ind = stringFindUtf8(a, b);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    errs += 1;
  } else {
    oks += 1;
  }
  itDelete(a); itDelete(b);

  a = itCreateString("mömömö");
  stringPartUtf8(a, &b, 2, 2);
  pStr=(char *)b->buf;
  if (strcmp("mö", pStr)) {
    printf("ERROR: part string, expected %s, got %s\n", "mö", pStr);
  } else {
    oks += 1;
  }
  itDelete(a); itDelete(b);
  
  /*
  stringFromCharString(&a, "momomo");
  stringPartBytes(&a, &b, 2, 2);
  stringPrint(&a); printf(" "); stringPrintLn(&b);
  long cnt = stringFindCountBytes(&a, &b);
  printf("Tok-count: %ld (3)\n", cnt);

  long idx = stringFindBytes(&a, &b, 0);
  printf("First tok: %ld (0)\n", idx);
  idx = stringFindBytes(&a, &b, 1);
  printf("First tok (off=1): %ld (2)\n", idx);
  idx = stringFindBytes(&a, &b, 3);
  printf("First tok (off=3): %ld (4)\n", idx);
  idx = stringFindBytes(&a, &b, 5);
  printf("First tok (off=5): %ld (-1)\n", idx);

  stringFromCharString(&a, "7777777");
  stringPartBytes(&a, &b, 2, 1);
  stringPrint(&a); printf(" "); stringPrintLn(&b);
  cnt = stringFindCountBytes(&a, &b);
  printf("Tok-count: %ld (7)\n", cnt);
*/
  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;
}

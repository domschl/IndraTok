// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_tok.h"
#include "crc_crypt_hash.h"

bool makeString(String *a, const char *str) {
  //printf("Creating string: %s\n", str);
  stringFromCharString(a, str);
  if (!stringValidateUtf8(a)) {
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
  String a = {0, NULL};
  String b = {0, NULL};
  String c = {0, NULL};
  
  if (!makeString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    errs += 1;
  } else {
    oks += 1;
  }
  // printf("String: "); stringPrintLn(&a); printf(" | Part 5,3: ");
  stringPartBytes(&a, &b, 5, 3);
  // stringPrintLn(&b);
  char *pStr = stringToCharStringAlloc(&b);
  if (strcmp(", w", pStr)) {
    printf("ERROR: part expected to be >, w<, got: >%s<.\n", pStr);
    errs += 1;
  } else {
    oks += 1;
  }
  if (pStr) free(pStr);

  stringFromCharString(&a, "Hello, ");
  stringFromCharString(&b, "world!");
  stringAppend(&a, &b);
  stringPrintLn(&a);
  pStr = stringToCharStringAlloc(&a);
  if (strcmp("Hello, world!", pStr)) {
    errs += 1;
    printf("Expected >Hello, world!<, got: >%s<\n", pStr);
  } else {
    oks += 1;
  }
  if (pStr) free(pStr);

  stringFromCharString(&c, "");
  unsigned long sum=0;
  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    stringFromCharString(&a, test1[i].charString);
    pStr = stringToCharStringAlloc(&a);
    if (strcmp(pStr, test1[i].charString)) {
      printf("ERROR: Conversion cycle failed for >%s<, result >%s<\n", test1[i].charString, pStr);
      errs += 1;
    } else {
      oks += 1;
    }
    stringAppend(&c, &a); sum+=test1[i].utf8Len;
    free(pStr);
    // stringDisplayHex(&a);
    unsigned long len=stringLenUtf8(&a);
    if (len != test1[i].utf8Len) {
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", test1[i].charString, len, test1[i].utf8Len); 
      errs += 1;
    } else {
      oks += 1;
    }
    len = stringLenUtf8(&c);
    if (len != sum) {
      pStr = stringToCharStringAlloc(&c);
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", pStr, len, sum);
      free(pStr);
      errs += 1;
    } else {
      oks += 1;
    }
  }

  stringFromCharString(&a, "Hello, World!");
  stringFromCharString(&b, "orld!");
  long ind = stringFindUtf8(&a, &b);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    errs += 1;
  } else {
    oks += 1;
  }

  stringFromCharString(&a, "mömömö");
  stringPartUtf8(&a, &b, 2, 2);
  pStr=stringToCharStringAlloc(&b);
  if (strcmp("mö", pStr)) {
    printf("ERROR: part string, expected %s, got %s\n", "mö", pStr);
  } else {
    oks += 1;
  }
  free(pStr);

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
  
  stringFree(&a);
  stringFree(&b);
  stringFree(&c);
  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;
}

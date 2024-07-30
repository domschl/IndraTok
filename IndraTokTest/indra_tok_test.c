// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>

#include "indra_tok.h"
#include "crc_crypt_hash.h"

bool makeString(String *a, const char *str) {
  printf("Creating string: %s\n", str);
  stringFromCharString(a, str);
  if (!stringValidateUtf8(a)) {
    printf("Bad string: <%s>\n", str);
    return false;
  }
  return true;
}

typedef struct _char_conv_test {
  char *charString;
  unsigned int utf8Len;
} CharConvTest;

CharConvTest test1[] = {{ "Smörö", 5}, {"རྒྱུད་", 6}, {"", 0}, {"𐍈", 1}, {"😁", 1}};

int main(int argc, char *argv[]) {
  unsigned int errs = 0;
  String a = {0, NULL};
  String b = {0, NULL};
  String c = {0, NULL};
  
  if (!makeString(&a, "Hello, world!")) errs += 1;
  stringPrintLn(&a);
  stringPartBytes(&a, &b, 5, 3);
  stringPrintLn(&b);

  stringFromCharString(&a, "Hello, ");
  stringFromCharString(&b, "world!");
  stringAppend(&a, &b);
  stringPrintLn(&a);

  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    stringFromCharString(&a, test1[i].charString);
    stringDisplayHex(&a);
    unsigned int len=stringLenUtf8(&a);
    if (len == test1[i].utf8Len) {
      printf("Correct utf8-length for >%s<, len=%u\n", test1[i].charString, len); 
    } else {
      printf("WRONG utf8-length for >%s<, got len=%u, expected %u\n", test1[i].charString, len, test1[i].utf8Len); 
      errs += 1;
    }
  }

  stringFromCharString(&a, "Hello, World!");
  stringFromCharString(&b, "orld!");
  int ind = stringFindUtf8(&a, &b);
  printf("Ind=%d\n", ind);

  stringFromCharString(&a, "mömömö");
  stringPartUtf8(&a, &b, 2, 2);
  printf("Part: "); stringPrintLn(&b);

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

  
  stringFree(&a);
  stringFree(&b);
  stringFree(&c);
  printf("\nErrors: %u\n", errs);
  return errs;
}

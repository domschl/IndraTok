// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_atom.h"
#include "indra_tok.h"

#include <sys/time.h>

bool makeString(IndraAtom **ppa, const char *str) {
  //printf("Creating string: %s\n", str);
  (*ppa) = iaCreateString(str);
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

typedef struct _tok_parse_test {
  char *str;
  char *tok;
} TokParseTest;

TokParseTest test2[] = {{"asdfjiefjiwjef", "asef"}, {"aaaa", "a"},
{"a", "a"}, {"", "a"}, {"a" ,""},
{"aaaaa", "a"},{"aaaaaaaaaaaaaaaa", "a"},
{"abcabcabc", "ab"}, {"abcabc", "abc"},
{"gulpabbagulpbubugulp", "gulp"},
{"abbagulpbubugulp", "gulp"},
{"gulpabbagulpbubu", "gulp"},
};

int main(int argc, char *argv[]) {
  struct timeval start, stop;
  unsigned int errs = 0, oks=0;
  IndraAtom *a=NULL, *b=NULL, *c=NULL;
  if (!makeString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    errs += 1;
  } else {
    oks += 1;
  }
  // printf("String: "); stringPrintLn(&a); printf(" | Part 5,3: ");
  b = stringPartUtf8(a, 5, 3);
  // stringPrintLn(&b);
  if (b==NULL || b->buf == NULL) {
    errs += 1;
    printf("ERROR: failed to get part\n");
  } else {
    char *pStr = (char *)b->buf;
    if (strcmp(", w", pStr)) {
      printf("ERROR: part expected to be >, w<, got: >%s<.\n", pStr);
      errs += 1;
    } else {
      oks += 1;
    }
  }
  iaDelete(b);
  iaDelete(a);
  
  a = iaCreateString("Hello, ");
  b = iaCreateString("world!");
  iaJoin(&a, b);
  iaPrintLn(a);
  char *pStr = (char *)a->buf;
  if (strcmp("Hello, world!", pStr)) {
    errs += 1;
    printf("Expected >Hello, world!<, got: >%s<\n", pStr);
  } else {
    oks += 1;
  }
  iaDelete(a);
  iaDelete(b);
  
  c = iaCreateString("");
  unsigned long sum=0;
  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    a = iaCreateString(test1[i].charString);
    pStr = (char *)a->buf;
    if (strcmp(pStr, test1[i].charString)) {
      printf("ERROR: Conversion cycle failed for >%s<, result >%s<\n", test1[i].charString, pStr);
      errs += 1;
    } else {
      oks += 1;
    }
    iaJoin(&c, a); sum+=test1[i].utf8Len;
    stringDisplayHex(a);
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
    iaDelete(a);
  }
  iaDelete(c);

  a = iaCreateString("Hello, World!");
  b = iaCreateString("orld!");
  long ind = stringFindUtf8(a, b);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(a);
  iaDelete(b);

  a = iaCreateString("mömömö");
  b = stringPartUtf8(a, 2, 2);
  pStr=(char *)b->buf;
  if (strcmp("mö", pStr)) {
    printf("ERROR: part string, expected %s, got %s\n", "mö", pStr);
  } else {
    oks += 1;
  }
  long cnt = stringFindCountUtf8(a, b);
  if (cnt != 3) {
    printf("ERROR: token count, got %ld, expected 3\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(a);
  iaDelete(b);

  
  a = iaCreateBytes((unsigned char *)"momomo", 6);
  b = iaSlice(a, 2, 2);
  if (memcmp((unsigned char *)b->buf, (unsigned char *)"mo", 2)) {
    errs += 1;
    printf("ERROR: part expected <mo>, got: "); iaPrint(b);
  } else {
    oks +=1;
  }
  iaPrint(a); printf(" "); iaPrintLn(b);
  cnt = stringFindCountBytes(a, b);
  if (cnt != 3) {
    printf("ERROR: Tok-count: %ld (3)\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }

  long idx = stringFindBytes(a, b, 0);
  printf("First tok: %ld (0)\n", idx);
  if (idx != 0) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  }
  idx = stringFindBytes(a, b, 1);
  printf("First tok (off=1): %ld (2)\n", idx);
  if (idx!=2) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  idx = stringFindBytes(a, b, 3);
  printf("First tok (off=3): %ld (4)\n", idx);
  if (idx!=4) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  idx = stringFindBytes(a, b, 5);
  printf("First tok (off=5): %ld (-1)\n", idx);
  if (idx != -1) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  iaDelete(a); iaDelete(b);
  
  a = iaCreateString("7777777");
  b = stringPartUtf8(a, 2, 1);
  iaPrint(a); printf(" "); iaPrintLn(b);
  if (strcmp(b->buf, "7")) {
    printf("ERROR string part, expected >7<, got: "); iaPrint(b);
    errs+=1;
  } else {
    oks+=1;
  }
  cnt = stringFindCountUtf8(a, b);
  printf("Tok-count: %ld (7)\n", cnt);
  if (cnt!=7) {
    printf("ERROR: wrong count, expected 7, got: %lu\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(a);
  iaDelete(b);

  a = iaCreateString("for|you|more");
  IndraAtom *ar;
  b = iaCreateString("|");
  ar = stringSplitUtf8(a, b);
  iaPrintLn(ar);
  
  iaDelete(a);
  iaDelete(b);
  iaDelete(ar);

  for (unsigned long i=0; i<sizeof(test2)/sizeof(TokParseTest); i++) {
    printf("Tokparse >%s< token >%s<\n", test2[i].str, test2[i].tok);
    a = iaCreateString(test2[i].str);
    b = iaCreateString(test2[i].tok);
    ar = stringSplitUtf8(a, b);
    iaPrintLn(ar);
    iaDelete(a);
    iaDelete(b);
    iaDelete(ar);    
  }

  
  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;
}

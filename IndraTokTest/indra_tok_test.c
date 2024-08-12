// -*- coding: utf-8 -*-
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

// #define IA_STACK_PREFERENCE 4

#include "indra_atom.h"
#include "indra_tok.h"

#include <sys/time.h>

typedef struct _char_conv_test {
  char *charString;
  unsigned long utf8Len;
} CharConvTest;

CharConvTest test1[] = {{ "Smörö", 5}, {"རྒྱུད་", 6}, {"", 0},
                    {"𐍈", 1}, {"😁", 1}};

typedef struct _tok_parse_test {
  char *str;
  char *tok;
} TokParseTest;

TokParseTest test2[] = {
{"babababab", "a"},
{"a", "a"},
{"", "a"},
{"a" ,""},
{"aaaaa", "a"},{"aaaaaaaaaaaaaaaa", "a"},
{"abcabcabc", "ab"}, {"abcabc", "abc"},
{"gulpabbagulpbubugulp", "gulp"},

{"asdfjiefjiwjef", "asef"}, 
{"Guten Morgen liebe Sorgen", "rg"},
{"abbagulpbubugulp", "gulp"},
{"gulpabbagulpbubu", "gulp"},
{"Das ist das lange Geschichten Buch, ohne Anfang und Ende", "n"},
};

int main(int argc, char *argv[]) {
  struct timeval start, stop;
  char *pStr;
  unsigned int errs = 0, oks=0;
  IA_T_ATOM a, b, c;
  if (!iaSetString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    errs += 1;
  } else {
    oks += 1;
  }
  printf("String: "); iaPrint(&a); printf(" | Part 5,3: ");

  bool res = iaStringUtf8Part(&a, &b, 5, 3);
  iaPrintLn(&b);
  if (!res) {
    errs += 1;
    printf("ERROR: failed to get part\n");
  } else {
    char *pStr = iaGetDataPtr(&b);
    if (strncmp(", w", pStr, 3)) {
      printf("ERROR: part expected to be >, w<, got: ");
      iaPrintLn(&b);
      errs += 1;
    } else {
      oks += 1;
    }
  }
  iaDelete(&b);
  iaDelete(&a);

  iaSetString(&a, "");
  printf("Empty string: >"); iaPrint(&a); printf("<\n");
  iaDelete(&a);
  
  iaSetString(&a, "Hello, ");
  iaSetString(&c, "world!");
  iaSetString(&b, "world!");
  iaJoin(&a, &b);
  iaPrintLn(&a);
  pStr = iaGetDataPtr(&a);
  if (strncmp("Hello, world!", pStr, strlen("Hello, world!"))) {
    errs += 1;
    printf("Expected >Hello, world!<, got: ");
    iaPrintLn(&a);
  } else {
    oks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  iaSetString(&c, "");
  unsigned long sum=0;
  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    iaSetString(&a, test1[i].charString);
    pStr = (char *)iaGetDataPtr(&a);
    if (strncmp(pStr, test1[i].charString, strlen(test1[i].charString))) {
      printf("ERROR: Conversion cycle failed for >%s<, result: ", test1[i].charString);
        iaPrint(&a); printf("\n");
      errs += 1;
    } else {
      printf("Conversion cycle ok for >%s<, result: ", test1[i].charString);
        iaPrint(&a); printf("\n");
      oks += 1;
    }
    iaJoin(&c, &a);
    sum+=test1[i].utf8Len;
    iaDelete(&a);

    iaSetString(&a, test1[i].charString);
    iaStringDisplayHex(&a);
    unsigned long len=iaStringUtf8Length(&a);
    if (len != test1[i].utf8Len) {
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", test1[i].charString, len, test1[i].utf8Len); 
      errs += 1;
    } else {
      oks += 1;
    }
    len = iaStringUtf8Length(&c);
    if (len != sum) {
      printf("WRONG utf8-length for >");
        iaPrint(&c);
      printf("<, got len=%lu, expected %lu\n", len, sum);
      errs += 1;
    } else {
      oks += 1;
    }
    iaDelete(&a);    
  }
  iaDelete(&c);

  iaSetString(&a,"Hello, World!");
  iaSetString(&b, "orld!");
  long ind = iaStringFind(&a, &b, 0);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  iaSetString(&a, "mömömö");
  iaStringUtf8Part(&a, &b, 2, 2);
  pStr=iaGetDataPtr(&b);
  if (strncmp("mö", pStr, strlen("mö"))) {
    printf("ERROR: part string, expected %s, got ", "mö");
    iaPrintLn(&b);
    errs += 1;
  } else {
    oks += 1;
  }
  long cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: token count, got %ld, expected 3\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  
  iaCreate(&a, IA_ID_CHAR, sizeof(char), 6, "momomo");
  iaSlice(&a, &b, 2, 2);
  if (memcmp(iaGetDataPtr(&b), (unsigned char *)"mo", 2)) {
    errs += 1;
    printf("ERROR: part expected <mo>, got: "); iaPrint(&b);
  } else {
    oks +=1;
  }
  iaPrint(&a); printf(" "); iaPrintLn(&b);
  cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: Tok-count: %ld (3)\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }

  long idx = iaStringFind(&a, &b, 0);
  printf("First tok: %ld (0)\n", idx);
  if (idx != 0) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  }
  idx = iaStringFind(&a, &b, 1);
  printf("First tok (off=1): %ld (2)\n", idx);
  if (idx!=2) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  idx = iaStringFind(&a, &b, 3);
  printf("First tok (off=3): %ld (4)\n", idx);
  if (idx!=4) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  idx = iaStringFind(&a, &b, 5);
  printf("First tok (off=5): %ld (-1)\n", idx);
  if (idx != -1) {
    printf("ERROR index");
    errs += 1;
  } else {
    oks += 1;
  } 
  iaDelete(&a); iaDelete(&b);
  
  iaSetString(&a, "7777777");
  iaStringUtf8Part(&a, &b, 2, 1);
  iaPrint(&a); printf(" "); iaPrintLn(&b);
  if (strncmp(iaGetDataPtr(&b), "7", b.count)) {
    printf("ERROR string part, expected >7<, got: "); iaPrint(&b);
    errs+=1;
  } else {
    oks+=1;
  }
  cnt = iaStringCountToken(&a, &b);
  printf("Tok-count: %ld (7)\n", cnt);
  if (cnt!=7) {
    printf("ERROR: wrong count, expected 7, got: %lu\n", cnt);
    errs += 1;
  } else {
    oks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);
  
  iaSetString(&a, "for|you|more");
  IA_T_ATOM *ar;
  iaSetString(&b, "|");
  iaStringUtf8Split(&a, &b, &c);
  iaPrintLn(&c);
  
  iaDelete(&a);
  iaDelete(&b);
  iaDelete(&c);

  for (unsigned long i=0; i<sizeof(test2)/sizeof(TokParseTest); i++) {
    printf("Tokparse >%s< token >%s<\n", test2[i].str, test2[i].tok);
    iaSetString(&a, test2[i].str);
    iaSetString(&b, test2[i].tok);
    if (iaStringUtf8Split(&a, &b, &c)) {
      iaPrintLn(&c);
      iaDelete(&c);    
    } else {
        printf("ERROR: split failed\n");
        errs += 1;
    }
    iaDelete(&a);
    iaDelete(&b);
  }

  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;  
}

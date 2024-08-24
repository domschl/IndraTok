// -*- coding: utf-8 -*-
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

// #define IA_STACK_PREFERENCE 4

#include "indra_atom.h"
#include "indra_tok.h"

#include <sys/time.h>

typedef struct _char_conv_test {
  char *testName;
  char *charString;
  unsigned long utf8Len;
} CharConvTest;

CharConvTest test1[] = {
  {"Latin", "Smörö", 5},
  {"Tibetan", "རྒྱུད་", 6},
  {"Empty", "", 0},
  {"Unicode-Symbal", "𐍈", 1},
  {"Smiley", "😁😁😁😁😁", 5},
  {" Amharic ", "ሰላም, እንኳን ወደ ዓለም መጣችሁ!",  22 },
  {" Arabic ", "مرحباً، أهلاً بك في العالم!",  27},
  {" Armenian ", "Բարև, բարի գալուստ աշխարհ!",  26 },
  {" Bengali ", "হ্যালো, বিশ্বে আপনাকে স্বাগতম!",  30 },
  {" Chinese ", "你好，欢迎来到这个世界！",  12 },
  {" English ", "Hello, welcome to the world!",  28 },
  {" Georgian ", "გამარჯობა, კეთილი იყოს თქვენი მობრძანება მსოფლიოში!",  51 },
  {" Greek ", "Γειά σας, καλώς ήρθατε στον κόσμο!",  34 },
  {" Hebrew ", "שלום, ברוכים הבאים לעולם!",  25},
  {" Hindi ", "नमस्ते, दुनिया में आपका स्वागत है!",  34 },
  {" Japanese ", "こんにちは、世界へようこそ！",  14 },
  {" Korean ", "안녕하세요, 세상에 오신 것을 환영합니다!",  23 },
  {" Russian ", "Здравствуйте, добро пожаловать в мир!",  37 },
  {" Spanish ", "¡Hola, bienvenido al mundo!",  27 },
  {" Swahili ", "Hujambo, karibu duniani!",  24 },
  {" Tagalog ", "Kamusta, maligayang pagdating sa mundo!",  39 },
  {" Thai ", "สวัสดี, ยินดีต้อนรับสู่โลก!",  27 },
  {" Vietnamese ", "Xin chào, chào mừng đến với thế giới!",  37 }
};

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

bool stringTests(int *poks, int *perrs, bool verbose) {
  IA_T_ATOM a,c;
  char *pStr;
  unsigned long sum=0;
  bool ok=true;
  iaSetString(&c, "");
  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    iaSetString(&a, test1[i].charString);
    pStr = (char *)iaGetDataPtr(&a);
    if (strncmp(pStr, test1[i].charString, strlen(test1[i].charString))) {
      printf("ERROR: Conversion cycle failed for >%s<, result: ", test1[i].charString);
      iaPrint(&a); printf("\n");
      *perrs += 1;
      ok=false;
    } else {
      if (verbose) {
        iaPrint(&a); printf("\n");
      }
      *poks += 1;
    }
    iaJoin(&c, &a);
    sum+=test1[i].utf8Len;
    iaDelete(&a);

    iaSetString(&a, test1[i].charString);
    if (verbose) iaStringDisplayHex(&a);
    unsigned long len=iaStringUtf8Length(&a);
    if (len != test1[i].utf8Len) {
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", test1[i].charString, len, test1[i].utf8Len); 
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }
    len = iaStringUtf8Length(&c);
    if (len != sum) {
      printf("WRONG utf8-length for >");
        iaPrint(&c);
      printf("<, got len=%lu, expected %lu\n", len, sum);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }
    iaDelete(&a);    
  }
  iaDelete(&c);
  return ok;
}

bool typeTests(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a;
  iaCreate(&a, IA_ID_CHAR, sizeof(char), 6, "momomo");
    if (a.type != IA_ID_CHAR) {
        printf("ERROR: type, expected %d, got %d\n", IA_ID_CHAR, a.type);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }
    char *pStr = iaGetDataPtr(&a);
    if (strncmp("momomo", pStr, 6)) {
        printf("ERROR: data, expected >momomo<, got >%s<\n", pStr);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }
    iaDelete(&a);
  iaCreate(&a, IA_ID_INT, sizeof(int), 3, (int[]){1, 2, 3});
    if (a.type != IA_ID_INT) {
        printf("ERROR: type, expected %d, got %d\n", IA_ID_INT, a.type);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }
    int *pInt = iaGetDataPtr(&a);
    if (pInt[0] != 1 || pInt[1] != 2 || pInt[2] != 3) {
        printf("ERROR: data, expected 1, 2, 3, got %d, %d, %d\n", pInt[0], pInt[1], pInt[2]);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }
    iaDelete(&a);
    iaCreate(&a, IA_ID_FLOAT, sizeof(float), 3, (float[]){1.1, 2.2, 3.3});
    if (a.type != IA_ID_FLOAT) {
        printf("ERROR: type, expected %d, got %d\n", IA_ID_FLOAT, a.type);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }
    float *pFloat = iaGetDataPtr(&a);
    if (pFloat[0] != 1.1 || pFloat[1] != 2.2 || pFloat[2] != 3.3) {
        printf("ERROR: data, expected 1.1, 2.2, 3.3, got %f, %f, %f\n", pFloat[0], pFloat[1], pFloat[2]);
        *perrs += 1;
        ok=false;
    } else {
        *poks += 1;
    }

  return ok;
}


bool oldTest(int *poks, int *perrs) {
  struct timeval start, stop;
  char *pStr;
  *perrs = 0;
  *poks=0;
  IA_T_ATOM a, b, c;
  if (!iaSetString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    *perrs += 1;
  } else {
    *poks += 1;
  }
  printf("String: "); iaPrint(&a); printf(" | Part 5,3: ");

  bool res = iaStringUtf8Part(&a, &b, 5, 3);
  iaPrintLn(&b);
  if (!res) {
    *perrs += 1;
    printf("ERROR: failed to get part\n");
  } else {
    char *pStr = iaGetDataPtr(&b);
    if (strncmp(", w", pStr, 3)) {
      printf("ERROR: part expected to be >, w<, got: ");
      iaPrintLn(&b);
      *perrs += 1;
    } else {
      *poks += 1;
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
    *perrs += 1;
    printf("Expected >Hello, world!<, got: ");
    iaPrintLn(&a);
  } else {
    *poks += 1;
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
      *perrs += 1;
    } else {
      printf("Conversion cycle ok for >%s<, result: ", test1[i].charString);
        iaPrint(&a); printf("\n");
      *poks += 1;
    }
    iaJoin(&c, &a);
    sum+=test1[i].utf8Len;
    iaDelete(&a);

    iaSetString(&a, test1[i].charString);
    iaStringDisplayHex(&a);
    unsigned long len=iaStringUtf8Length(&a);
    if (len != test1[i].utf8Len) {
      printf("WRONG utf8-length for >%s<, got len=%lu, expected %lu\n", test1[i].charString, len, test1[i].utf8Len); 
      *perrs += 1;
    } else {
      *poks += 1;
    }
    len = iaStringUtf8Length(&c);
    if (len != sum) {
      printf("WRONG utf8-length for >");
        iaPrint(&c);
      printf("<, got len=%lu, expected %lu\n", len, sum);
      *perrs += 1;
    } else {
      *poks += 1;
    }
    iaDelete(&a);    
  }
  iaDelete(&c);

  iaSetString(&a,"Hello, World!");
  iaSetString(&b, "orld!");
  long ind = iaStringFind(&a, &b, 0);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    *perrs += 1;
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  iaSetString(&a, "mömömö");
  iaStringUtf8Part(&a, &b, 2, 2);
  pStr=iaGetDataPtr(&b);
  if (strncmp("mö", pStr, strlen("mö"))) {
    printf("ERROR: part string, expected %s, got ", "mö");
    iaPrintLn(&b);
    *perrs += 1;
  } else {
    *poks += 1;
  }
  long cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: token count, got %ld, expected 3\n", cnt);
    *perrs += 1;
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  
  iaCreate(&a, IA_ID_CHAR, sizeof(char), 6, "momomo");
  iaSlice(&a, &b, 2, 2);
  if (memcmp(iaGetDataPtr(&b), (unsigned char *)"mo", 2)) {
    *perrs += 1;
    printf("ERROR: part expected <mo>, got: "); iaPrint(&b);
  } else {
    *poks +=1;
  }
  iaPrint(&a); printf(" "); iaPrintLn(&b);
  cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: Tok-count: %ld (3)\n", cnt);
    *perrs += 1;
  } else {
    *poks += 1;
  }

  long idx = iaStringFind(&a, &b, 0);
  printf("First tok: %ld (0)\n", idx);
  if (idx != 0) {
    printf("ERROR index");
    *perrs += 1;
  } else {
    *poks += 1;
  }
  idx = iaStringFind(&a, &b, 1);
  printf("First tok (off=1): %ld (2)\n", idx);
  if (idx!=2) {
    printf("ERROR index");
    *perrs += 1;
  } else {
    *poks += 1;
  } 
  idx = iaStringFind(&a, &b, 3);
  printf("First tok (off=3): %ld (4)\n", idx);
  if (idx!=4) {
    printf("ERROR index");
    *perrs += 1;
  } else {
    *poks += 1;
  } 
  idx = iaStringFind(&a, &b, 5);
  printf("First tok (off=5): %ld (-1)\n", idx);
  if (idx != -1) {
    printf("ERROR index");
    *perrs += 1;
  } else {
    *poks += 1;
  } 
  iaDelete(&a); iaDelete(&b);
  
  iaSetString(&a, "7777777");
  iaStringUtf8Part(&a, &b, 2, 1);
  iaPrint(&a); printf(" "); iaPrintLn(&b);
  if (strncmp(iaGetDataPtr(&b), "7", b.count)) {
    printf("ERROR string part, expected >7<, got: "); iaPrint(&b);
    *perrs+=1;
  } else {
    *poks+=1;
  }
  cnt = iaStringCountToken(&a, &b);
  printf("Tok-count: %ld (7)\n", cnt);
  if (cnt!=7) {
    printf("ERROR: wrong count, expected 7, got: %lu\n", cnt);
    *perrs += 1;
  } else {
    *poks += 1;
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
        *perrs += 1;
    }
    iaDelete(&a);
    iaDelete(&b);
  }
  if (*perrs) {
    return false;
  }
  return true;
}

bool simpleMatrix(int *poks, int *perrs) {
  *perrs = 0;
  *poks = 0;
  IA_T_ATOM a, b, c;
  int vec1[]={1, 2, 3};
  int vec2[]={4, 5, 6};
  iaCreate(&a, IA_ID_INT, sizeof(int), 3, &vec1);
  iaCreate(&b, IA_ID_INT, sizeof(int), 3, &vec2);
  iaSetAtom(&c, &a);
  iaAppend(&c, &b);
  iaPrint(&c);
  iaDelete(&a);
  iaDelete(&b);
  iaDelete(&c);
  if (*perrs) {
    return false;
  }
    return true;
}

bool simpleTensor(int *poks, int *perrs) {
  *perrs = 0;
  *poks = 0;
  IA_T_ATOM a, b, c, d;
  int vec1[]={1, 2, 3};
  int vec2[]={4, 5, 6};
  printf("Tensor test\n");
  iaCreate(&a, IA_ID_INT, sizeof(int), 3, &vec1);
  iaCreate(&b, IA_ID_INT, sizeof(int), 3, &vec2);
  iaSetAtom(&c, &a);
  iaAppend(&c, &b);
  iaPrintLn(&c);
  iaDelete(&a);
  iaDelete(&b);
  printf("c: "); iaPrintLn(&c);
  iaCopy(&c, &a);
  printf("params: a: "); iaPrintLn(&a);
  iaCopy(&c, &b);
  printf("params: b: "); iaPrintLn(&b);
  iaDelete(&c);
  printf("moving a: "); iaPrint(&a); printf(" to d\n");
  iaSetAtom(&d, &a);
  printf("params: d: "); iaPrintLn(&d);
  printf("appending b to d\n");
  iaAppend(&d, &b);
  printf("params: [a,b]: ");
  iaPrintLn(&d);
  iaDelete(&a);
  iaDelete(&b);
  iaCreate(&a, IA_ID_INT, sizeof(int), 3, (int[]){7, 8, 9, 10});
  printf("params: a (%ld): ", a.count); iaPrintLn(&a);
  printf("appending a to d (%ld): ", d.count); iaPrint(&d); printf("\n");
  iaJoin(&d, &a);
  printf("params: [d, a] (%ld): ", d.count); iaPrintLn(&d);
  iaDelete(&a);
  iaSlice(&d, &a, 2, 1);
  printf("Slice: "); iaPrintLn(&a);
  iaSlice(&a, &b, 0, 1);
  printf("Slice: "); iaPrintLn(&b);
  iaDelete(&b);
  iaDelete(&a);
  iaDelete(&d);
  if (*perrs) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  int errs=0, oks=0;
  bool verbose=false;
  /*
  if (!oldTest(&oks, &errs)) {
    printf("Old test failed\n");
  }
  if (!simpleMatrix(&oks, &errs)) {
    printf("Simple matrix test failed\n");
  }
    if (!simpleTensor(&oks, &errs)) {
        printf("Simple tensor test failed\n");
    }
  */
  if (!stringTests(&oks, &errs, verbose)) {
    printf("String tests failed\n");
  }
  if (!typeTests(&oks, &errs, verbose)) {
    printf("Type tests failed\n");
  }
  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;  
}

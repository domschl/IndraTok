// -*- coding: utf-8 -*-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "indra_args.h"
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
  {" Tibetan ", "ཝ་ཡེ། འཛམ་གླིང་འདིར་ཕེབས་པར་དགའ་བསུ་ཞུ།", 39},
  {" Vietnamese ", "Xin chào, chào mừng đến với thế giới!",  37 }
};

typedef struct _tok_parse_test {
  char *str;
  char *tok;
  int tokcount;
} TokParseTest;

TokParseTest test2[] = {
  {"babababab", "a", 5},
  {"a", "a", 1},
  {"", "a", 1},
  {"a" ,"", 2},
  {"abc" ,"", 4},
  {"aaaaa", "a", 5},
  {"aaaaaaaaaaaaaaaa", "a", 16},
  {"abcabcabc", "ab", 4},
  {"abcabc", "abc",2},
  {"gulpabbagulpbubugulp", "gulp", 3},
  {"asdfjiefjiwjef", "asef", 1}, 
  {"Guten Morgen liebe Sorgen", "rg", 3},
  {"abbagulpbubugulp", "gulp", 2},
  {"gulpabbagulpbubu", "gulp", 3},
  {"Das ist das lange Geschichten Buch, ohne Anfang und Ende", "n", 8},
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

  int intTestLen = 30;
  int testInt[intTestLen];
  for (int i=0; i<intTestLen; i++) testInt[i] = rand();

  iaCreate(&a, IA_ID_INT, sizeof(int), intTestLen, testInt);
  if (a.type != IA_ID_INT) {
    printf("ERROR: type, expected %d, got %d\n", IA_ID_INT, a.type);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  int *pInt = iaGetDataPtr(&a);
  for (int i=0; i<intTestLen; i++) {
    if (pInt[i] != testInt[i]) {
      printf("ERROR: data[%d], expected %d, got %d\n", i, testInt[i], pInt[i]);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }      
  }
  iaDelete(&a);

  iaCreate(&a, IA_ID_INT, sizeof(int), 0, NULL);
  for (int i=0; i<intTestLen; i++) iaAppend(&a, &testInt[i]);
  if (a.type != IA_ID_INT) {
    printf("ERROR: type, expected %d, got %d\n", IA_ID_INT, a.type);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  pInt = iaGetDataPtr(&a);
  for (int i=0; i<intTestLen; i++) {
    if (pInt[i] != testInt[i]) {
      printf("ERROR: data[%d], expected %d, got %d\n", i, testInt[i], pInt[i]);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }      
  }
  iaDelete(&a);

  int floatTestLen = 30;
  float testFloat[floatTestLen];
  for (int i=0; i<floatTestLen; i++) testFloat[i] = (float)rand()/(float)RAND_MAX*100.0;
  iaCreate(&a, IA_ID_FLOAT, sizeof(float), floatTestLen, testFloat);
  if (a.type != IA_ID_FLOAT) {
    printf("ERROR: type, expected %d, got %d\n", IA_ID_FLOAT, a.type);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  float feps = 1e-5;
  float *pFloat = iaGetDataPtr(&a);
  for (int i=0; i<intTestLen; i++) {
    if (fabs(pFloat[i] - testFloat[i]) > feps) {
      printf("ERROR: data[%d], expected %f, got %f\n", i, testFloat[i], pFloat[i]);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }      
  }
  iaDelete(&a);
    
  iaCreate(&a, IA_ID_FLOAT, sizeof(float), 0, NULL);
  for (int i=0; i<floatTestLen; i++) iaAppend(&a, &testFloat[i]);
  if (a.type != IA_ID_FLOAT) {
    printf("ERROR: type, expected %d, got %d\n", IA_ID_FLOAT, a.type);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  feps = 1e-5;
  pFloat = iaGetDataPtr(&a);
  for (int i=0; i<intTestLen; i++) {
    if (fabs(pFloat[i] - testFloat[i]) > feps) {
      printf("ERROR: data[%d], expected %f, got %f\n", i, testFloat[i], pFloat[i]);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }      
  }
  iaDelete(&a);
    
  return ok;
}

bool simpleStringTests(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a,b,c;
  char *pStr;
  if (!iaSetString(&a, "Hello, world!")) {
    printf("ERROR: Failed to create string.\n");
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  if (verbose) {
    printf("String: "); iaPrint(&a); printf(" | Part 5,3: ");
  }

  bool res = iaStringUtf8Part(&a, &b, 5, 3);
  if (verbose) iaPrintLn(&b);
  if (!res) {
    *perrs += 1;
    ok=false;
    printf("ERROR: failed to get part\n");
  } else {
    char *pStr = iaGetDataPtr(&b);
    if (strncmp(", w", pStr, 3)) {
      printf("ERROR: part expected to be >, w<, got: ");
      iaPrintLn(&b);
      *perrs += 1;
      ok=false;
    } else {
      *poks += 1;
    }
  }
  iaDelete(&b);
  iaDelete(&a);

  iaSetString(&a, "");
  if (verbose) {
    printf("Empty string: >"); iaPrint(&a); printf("<\n");
  }
  iaDelete(&a);
  
  iaSetString(&a, "Hello, ");
  iaSetString(&c, "world!");
  iaSetString(&b, "world!");
  iaJoin(&a, &b);
  if (verbose) iaPrintLn(&a);
  pStr = iaGetDataPtr(&a);
  if (strncmp("Hello, world!", pStr, strlen("Hello, world!"))) {
    *perrs += 1;
    ok=false;
    printf("Expected >Hello, world!<, got: ");
    iaPrintLn(&a);
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);
  return ok;
}

bool simpleUtf8Tests(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a,b;
  long cnt;
  iaCreate(&a, IA_ID_CHAR, sizeof(char), 6, "momomo");
  iaSlice(&a, &b, 2, 2);
  if (memcmp(iaGetDataPtr(&b), (unsigned char *)"mo", 2)) {
    *perrs += 1;
    ok=false;
    printf("ERROR: part expected <mo>, got: "); iaPrint(&b);
  } else {
    *poks +=1;
  }
  if (verbose) {
    iaPrint(&a); printf(" "); iaPrintLn(&b);
  }
  cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: Tok-count: %ld (3)\n", cnt);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }

  long idx = iaStringFind(&a, &b, 0);
  if (verbose) printf("First tok: %ld (0)\n", idx);
  if (idx != 0) {
    printf("ERROR index");
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  idx = iaStringFind(&a, &b, 1);
  if (verbose) printf("First tok (off=1): %ld (2)\n", idx);
  if (idx!=2) {
    printf("ERROR index");
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  } 
  idx = iaStringFind(&a, &b, 3);
  if (verbose) printf("First tok (off=3): %ld (4)\n", idx);
  if (idx!=4) {
    printf("ERROR index");
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  } 
  idx = iaStringFind(&a, &b, 5);
  if (verbose) printf("First tok (off=5): %ld (-1)\n", idx);
  if (idx != -1) {
    printf("ERROR index");
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  } 
  iaDelete(&a); iaDelete(&b);
  
  iaSetString(&a, "7777777");
  iaStringUtf8Part(&a, &b, 2, 1);
  if (verbose) {
    iaPrint(&a); printf(" "); iaPrintLn(&b);
  }
  if (strncmp(iaGetDataPtr(&b), "7", b.count)) {
    printf("ERROR string part, expected >7<, got: "); iaPrint(&b);
    *perrs+=1;
    ok=false;
  } else {
    *poks+=1;
  }
  cnt = iaStringCountToken(&a, &b);
  if (verbose) printf("Tok-count: %ld (7)\n", cnt);
  if (cnt!=7) {
    printf("ERROR: wrong count, expected 7, got: %lu\n", cnt);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);
  return ok;
}

bool utf8FindCount(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  char *pStr;
  IA_T_ATOM a,b,c;
  iaSetString(&a,"Hello, World!");
  iaSetString(&b, "orld!");
  long ind = iaStringFind(&a, &b, 0);
  if (ind != 8) {
    printf("ERROR: findUtf8, expected 8, got %ld\n", ind);
    *perrs += 1;
    ok=false;
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
    ok =false;
  } else {
    *poks += 1;
  }
  long cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: token count, got %ld, expected 3\n", cnt);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  
  iaCreate(&a, IA_ID_CHAR, sizeof(char), 6, "momomo");
  iaSlice(&a, &b, 2, 2);
  if (memcmp(iaGetDataPtr(&b), (unsigned char *)"mo", 2)) {
    *perrs += 1;
    ok=false;
    printf("ERROR: part expected <mo>, got: "); iaPrint(&b);
  } else {
    *poks +=1;
  }
  if (verbose) {
    iaPrint(&a); printf(" "); iaPrintLn(&b);
  }
  cnt = iaStringCountToken(&a, &b);
  if (cnt != 3) {
    printf("ERROR: Tok-count: %ld (3)\n", cnt);
    *perrs += 1;
    ok=false;
  } else {
    *poks += 1;
  }
  
  iaSetString(&a, "7777777");
  iaStringUtf8Part(&a, &b, 2, 1);
  if (verbose) {
    iaPrint(&a); printf(" "); iaPrintLn(&b);
  }
  if (strncmp(iaGetDataPtr(&b), "7", b.count)) {
    printf("ERROR string part, expected >7<, got: "); iaPrint(&b);
    *perrs+=1;
  } else {
    *poks+=1;
  }
  cnt = iaStringCountToken(&a, &b);
  if (verbose) printf("Tok-count: %ld (7)\n", cnt);
  if (cnt!=7) {
    printf("ERROR: wrong count, expected 7, got: %lu\n", cnt);
    *perrs += 1;
  } else {
    *poks += 1;
  }
  iaDelete(&a);
  iaDelete(&b);

  
  iaSetString(&a, "for|you|more");
  iaSetString(&b, "|");
  iaStringUtf8Split(&a, &b, &c);
  if (verbose) iaPrintLn(&c);
  if (c.count != 3) {
    *perrs += 1;
    ok=false;
    printf("Wrong token count, expected 3, got %ld", c.count);
  } else {
    *poks += 1;
    if (strncmp("for", iaGetDataPtr(iaGetIndexPtr(&c,0)), 3) || strncmp("you", iaGetDataPtr(iaGetIndexPtr(&c, 1)), 3) || strncmp("more", iaGetDataPtr(iaGetIndexPtr(&c, 2)), 4)) {
      *perrs += 1;
      ok=false;
      printf("Got wrong tokens, expected for, you, more, got: "); iaPrintLn(&c);
      
    } else {
      *poks+=1;
    }
  }
  
  iaDelete(&a);
  iaDelete(&b);
  iaDelete(&c);
  return ok;
}

bool tokTest(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a,b,c;

  for (unsigned long i=0; i<sizeof(test2)/sizeof(TokParseTest); i++) {
    if (verbose) printf("Tokparse >%s< token >%s<\n", test2[i].str, test2[i].tok);
    iaSetString(&a, test2[i].str);
    iaSetString(&b, test2[i].tok);
    if (iaStringUtf8Split(&a, &b, &c)) {
      if (verbose) iaPrintLn(&c);
      *poks+=1;
      if (c.count != test2[i].tokcount) {
        printf("Tokparse of >%s< for token >%s< got %ld tokens, expected %d\n", test2[i].str, test2[i].tok, c.count, test2[i].tokcount);
        printf("Got: "); iaPrintLn(&c);
        *perrs += 1;
        ok=false;
      } else {
        *poks+=1;
      }
      iaDelete(&c);    
    } else {
      printf("ERROR: split failed\n");
      *perrs += 1;
      ok=false;
    }
    iaDelete(&a);
    iaDelete(&b);
  }
  if (*perrs) {
    return false;
  }
  return true;
}

bool simpleMatrix(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a, b, c;
  int vec[2][3]={{1, 2, 3}, {4, 5, 6}};
  int xy;
  iaCreate(&a, IA_ID_INT, sizeof(int), 3, &vec[0]);
  iaCreate(&b, IA_ID_INT, sizeof(int), 3, &vec[1]);
  iaSetAtom(&c, &a);
  iaAppend(&c, &b);
  if (verbose) iaPrint(&c);
  for (int y=0; y<2; y++) {
    for (int x=0; x<3; x++) {
      xy = *(int *)iaGetIndexPtr2D(&c, y, x, true);
      if (xy != vec[y][x]) {
        printf("Wrong matrix content %d,%d, expected: %d, got %d\n", y,x, vec[y][x], xy);
        *perrs += 1;
        ok =false;
      } else {
        *poks += 1;
      }
    }
  }  
  iaDelete(&a);
  iaDelete(&b);
  iaDelete(&c);
  return ok;
}

bool simpleTensor(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  int val;
  IA_T_ATOM a, b, c, d, e, f;
  int ragged_tensor[3][2][3]={{{1,2,3}, {4,5,6}}, {{7,8,9}, {10,11,12}}, {13,14,15}};
  iaCreate(&a, IA_ID_INT, sizeof(IA_ID_INT), 3, &ragged_tensor[0][0]);
  iaCreate(&b, IA_ID_INT, sizeof(IA_ID_INT), 3, &ragged_tensor[0][1]);
  iaSetAtom(&c, &a);
  iaAppend(&c, &b);
  if (verbose) iaPrintLn(&c);
  iaDelete(&a);
  iaDelete(&b);
  if (verbose) {
    printf("c: ");
    iaPrintLn(&c);
  }

  iaCreate(&a, IA_ID_INT, sizeof(IA_ID_INT), 3, &ragged_tensor[1][0]);
  iaCreate(&b, IA_ID_INT, sizeof(IA_ID_INT), 3, &ragged_tensor[1][1]);
  iaSetAtom(&d, &a);
  iaAppend(&d, &b);
  if (verbose) iaPrintLn(&d);
  iaDelete(&a);
  iaDelete(&b);
  if (verbose) {
    printf("d: ");
    iaPrintLn(&d);
  }

  iaCreate(&e, IA_ID_INT, sizeof(int), 3, &ragged_tensor[2]);

  iaCreate(&f, IA_ID_ATOM, sizeof(IA_T_ATOM), 0, NULL);
  iaAppend(&f, &c);
  iaAppend(&f, &d);
  iaAppend(&f, &e);

  if (verbose) iaPrintLn(&f);

  for (int z=0; z<3; z++) {
    for (int y=0; y<3; y++) {
      for (int x=0; x<3; x++) {
        void *p = iaGetIndexPtr3D(&f, z, y, x, false);
        if (p!=NULL) {
          val = *(int *)p;
          if (verbose) printf("3D z=%d, y=%d, x=%d => val=%d\n", z, y, x, val);
          if (val != ragged_tensor[z][y][x]) {
            printf("ERROR: Ragged tensor, expected %d, got %d\n", ragged_tensor[z][y][x], val);
            *perrs += 1;
            ok=false;
          } else {
            *poks += 1;
          }
        } else {
          if (y==0) {
            void *p2 = iaGetIndexPtr2D(&f, z,  x, false);
            if (p2!=NULL) {
              val = *(int *)p2;
              if (verbose) printf("2D z=%d, y=%d, x=%d => val=%d\n", z, y, x, val);
              if (val != ragged_tensor[z][y][x]) {
                printf("ERROR: Ragged tensor, expected %d, got %d\n", ragged_tensor[z][y][x], val);
                *perrs += 1;
                ok=false;
              } else {
                *poks += 1;
              }
            }
          }
        }
      }
    }
  }
  
  iaDelete(&c);
  iaDelete(&d);
  iaDelete(&e);
  iaDelete(&f);
  return ok;
}

bool mapTest(int *poks, int *perrs, bool verbose) {
  bool ok=true;
  IA_T_ATOM a,b,c;
  IA_T_MAP map;
  iaCreateMap(&map, 1024);
  iaSetString(&a, "Test");
  iaSetString(&b, "Value");
  if (iaMapSet(&map, &a, &b)) {
    if (iaMapGet(&map, &a, &c)) {
      unsigned long size_b, size_c;
      size_b = iaGetRecsize(&b) * b.count;
      size_c = iaGetRecsize(&c) * b.count;
      if (size_b == size_c && !memcmp(iaGetDataPtr(&b), iaGetDataPtr(&c), size_b)) {
        *poks += 1;
      } else {
        *perrs += 1;
        ok = false;
        printf("Retrieval failure!");
        printf("Correct: "); iaPrintLn(&b);
        printf("Got: "); iaPrintLn(&c);
      }
      // iaDelete(&c);
    } else {
      *perrs += 1;
      ok = false;
      printf("Key not found\n");
    }
  } else {
    *perrs += 1;
    ok = false;
    printf("Error setting key/value");
  }

  iaDelete(&a);
  iaDelete(&b);
  iaMapDelete(&map);


  unsigned long N=1000000;
  iaCreateMap(&map, N*2);

  printf("Test 1\n");
  for (unsigned long i=0; i<N; i++) {
    iaSetInt(&a, i);
    iaSetInt(&b, i*3);
    iaMapSet(&map, &a, &b);
    iaDelete(&a);
    iaDelete(&b);
  }
  
  for (unsigned long i=0; i<N; i++) {
    iaSetInt(&a, i);
    iaMapRemove(&map, &a);
    iaDelete(&a);
    }
   
  iaMapDelete(&map);

  printf("Test 2\n");
  iaCreateMap(&map, 1700000);
  char buf[64];
  for (unsigned long i=0; i<N; i++) {
    sprintf(buf, "Key in question %lu", i);
    iaSetString(&a, buf);
    sprintf(buf, "Value that precisely describes the state: %lu", i*4);
    iaSetString(&b, buf);
    iaMapSet(&map, &a, &b);
    iaDelete(&a);
    iaDelete(&b);
  }
  for (unsigned long i=0; i<N; i++) {
    sprintf(buf, "Key in question %lu", i);
    iaSetString(&a, buf);
    sprintf(buf, "Value that precisely describes the state: %lu", i*4);
    iaSetString(&b, buf);
    if (iaMapGet(&map, &a, &c)) {
      if (c.type != IA_ID_CHAR || b.count != c.count || strncmp((char *)iaGetDataPtr(&b), (char *)iaGetDataPtr(&c), b.count)) {
        *perrs += 1;
        ok = false;
        printf("Error in map, expected: >");
        iaPrint(&b);
        printf("<, got: >");
        iaPrint(&c);
        printf("<\n");
      } else {
        *poks += 1;
      iaDelete(&c);
      }
    } else {
      *perrs += 1;
      ok = false;
      printf("Error in map, key %ld not found\n", i);
    }
    iaDelete(&b);
    iaDelete(&a);
  }
  iaMapDelete(&map);
  
  return ok;
}


int main(int argc, char *argv[]) {
  int errs=0, oks=0;
  bool verbose=false;
  IndraArg iad[] = {{"-v", "--verbose", BOOL, &verbose, 0, "Show verbose output"}};
  if (!indraArgParse(argc, argv, 1, iad)) {
    exit(-1);
  }
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
  if (!simpleStringTests(&oks, &errs, verbose)) {
    printf("Simple String tests failed\n");
  }
  if (!simpleUtf8Tests(&oks, &errs, verbose)) {
    printf("Simple Utf8 tests failed\n");
  }
  if (!stringTests(&oks, &errs, verbose)) {
    printf("String tests failed\n");
  }
  if (!typeTests(&oks, &errs, verbose)) {
    printf("Type tests failed\n");
  }
  if (!utf8FindCount(&oks, &errs, verbose)) {
    printf("Find Count test failed\n");
  }
  if (!tokTest(&oks, &errs, verbose)) {
    printf("Tok Test failed\n");
  }
  if (!simpleMatrix(&oks, &errs, verbose)) {
    printf("Simple matrix test failed\n");
  }
  if (!simpleTensor(&oks, &errs, verbose)) {
    printf("Simple Tensor test failed\n");
  }
  if (!mapTest(&oks, &errs, verbose)) {
    printf("Map Test failed\n");
  }
  printf("\nErrors: %u, Oks: %u\n", errs, oks);
  return errs;  
}

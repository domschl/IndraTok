#include <stdio.h>
#include <stdlib.h>

#include "indra_tok.h"

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

CharConvTest test1[] = {{ "Smörö", 5}, {"རྒྱུད་", 6}, {"", 0}, {"𐍈", 1}};

int main(int argc, char *argv[]) {
  unsigned int errs = 0;
  String a = {0, NULL};
  String b = {0, NULL};
  String c = {0, NULL};
  
  if (!makeString(&a, "Hello, world!")) errs += 1;
  stringPrintLn(&a);
  stringPartByte(&a, &b, 5, 3);
  stringPrintLn(&b);

  stringFromCharString(&a, "Hello, ");
  stringFromCharString(&b, "world!");
  stringAppend(&a, &b);
  stringPrintLn(&a);

  for (unsigned int i=0; i<sizeof(test1)/sizeof(test1[0]); i++) {
    stringFromCharString(&a, test1[i].charString);
    unsigned int len=stringLenUtf8(&a);
    if (len == test1[i].utf8Len) {
      printf("Correct utf8-length for >%s<, len=%u\n", test1[i].charString, len); 
    } else {
      printf("WRONG utf8-length for >%s<, got len=%u, expected %u\n", test1[i].charString, len, test1[i].utf8Len); 
      errs += 1;
    }
  }
  stringFree(&a);
  stringFree(&b);
  stringFree(&c);
  printf("\nErrors: %u\n", errs);
  return errs;
}

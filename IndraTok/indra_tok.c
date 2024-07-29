/*! \brief IndraTok: fast language agnostic tokenizer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_tok.h"

/*! Free memory allocated within String
  @param string, a pointer to a String instance
*/
void stringFree(String *string) {
  if (string == NULL) return;
  if (string->len>0) {
    if (string->buf != NULL) {
      free(string->buf);
      string->buf=(unsigned char *)NULL;
    }
    string->len=0;
  } else {
    string->buf=(unsigned char *)NULL;
  }
}

void stringFromCharString(String *string, const char* charString) {
  if (string==NULL || charString ==NULL) return;
  stringFree(string);
  if (strlen(charString)>0) {
    string->len = strlen(charString);
    string->buf = (unsigned char *)malloc(string->len);
    memcpy(string->buf, charString, string->len);
  }
}

char* stringToCharStringAlloc(const String *string) {
  if (string == NULL) return NULL;
  if (string->len == 0) return NULL;
  char *charString=(char *)malloc(string->len+1);
  memcpy(charString, string->buf, string->len);
  charString[string->len]=0;
  return charString;
}

void stringToCharStringN(const String *string, char *charString, unsigned int charStringMemSize) {
  if (string == NULL || charString == NULL) return;
  unsigned int n = string->len;
  if (string->len >= charStringMemSize) {
    n=charStringMemSize -1;
  }
  memset(charString, 0, charStringMemSize);
  memcpy(charString, string->buf, n);
}

void stringAppend(String *root, const String *appendix) {
  if (root == NULL || appendix == NULL) return;
  if (root->buf==NULL || appendix->buf==NULL) return;
  int n = root->len + appendix->len;
  unsigned char* oldRoot = (unsigned char *)malloc(root->len);
  memcpy(oldRoot, root->buf, root->len);
  free(root->buf);
  root->buf=(unsigned char *)malloc(n);
  memcpy(root->buf, oldRoot, root->len);
  free(oldRoot);
  memcpy(&root->buf[root->len], appendix->buf, appendix->len);
  root->len=n;
}

void stringPartByte(const String *source, String *part, unsigned int start, unsigned int len) {
  if (part == NULL) return;
  stringFree(part);
  if (source == NULL) return;
  if (start > source->len) return;
  unsigned int n = len;
  if (start + n > source->len) n = source->len - start;
  part->buf=(unsigned char*) malloc(n);
  part->len=n;
  memcpy(part->buf, &source->buf[start], n);
}

void stringStartByte(const String *source, String *start, unsigned int len) {
  stringPartByte(source, start, 0, len);
}

void stringEndByte(const String *source, String *end, unsigned int len) {
  if (end == NULL) return;
  stringFree(end);
  if (source == NULL) return;
  unsigned int start = source->len - len;
  if (start < 0) start = 0;
  stringPartByte(source, end, start, len);
}

bool stringContains(const String *source, const String *token) {
  if (token==NULL) return false;
  if (source==NULL) return false;
  unsigned int l=0;
  for (unsigned int s=0; s<source->len; s++) {
    if (l+1 > token->len) return true;
    if (source->buf[s] != token->buf[l]) continue;
    else l += 1;
  }
  if (l+1 > token->len) return true;
  else return false;
}

void stringPrint(const String *source) {
  if (source == NULL) return;
  if (source->len == 0) return;
  char *p = stringToCharStringAlloc(source);
  if (p != NULL) {
    printf("%s", p);
    free(p);
  } else {
    printf("<NULL>");
  }
}

void stringPrintLn(const String *source) {
  stringPrint(source);
  printf("\n");
}

inline unsigned int utf8CharLen(unsigned char ctok) {
  if ((ctok & 0x80) == 0) {
      return 1;
  } else if ((ctok & 0xF8) == 0xF0) {
    return 4;
  } else if ((ctok & 0xF0) == 0xE0) {
    return 3;
  } else if ((ctok & 0xE0) == 0xC0) {
    return 2;
  } else {
    return 0;
  }
}

bool stringValidateUtf8(const String *source) {
  if (source==NULL) return false;
  if (source->buf==NULL) return false;
  unsigned int charLen, j;
  for (unsigned int i=0; i<source->len; i++) {
    charLen = utf8CharLen(source->buf[i]);
    if (!charLen) return false;
    if (charLen==1) continue;
    if (i+charLen > source->len) return false;
    for (j=1; j<charLen; j++) {
      if ((source->buf[i+j] & 0xC0) != 0x80) return false;
    }
    i += charLen-1;
  }
  return true;
}

unsigned int stringLenUtf8(const String *source) {
  if (source==NULL) return 0;
  if (source->buf==NULL) return 0;
  unsigned int n=0, charLen;
  for (unsigned int i=0; i<source->len; i++) {
    charLen = utf8CharLen(source->buf[i]);
    if (charLen==0) return 0;
    n+=1;
    i+=charLen-1;
  }
  return n;
}


void stringDisplayHex(const String *source) {
  
}

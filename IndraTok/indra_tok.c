/*! \brief IndraTok: fast language agnostic tokenizer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_ent.h"


void stringAppend(IndraEnt *root, const IndraEnt *appendix) {
  if (root == NULL || appendix == NULL) return;
  if (root->type != IT_STRING || appendix->type!=IT_STRING) return;
  if (appendix->buf==NULL) return;
  int n = root->len + appendix->len;
  char* oldRoot = NULL;
  if (root->len) {
    oldRoot = (char *)malloc(root->len);
    memcpy(oldRoot, root->buf, root->len);
    free(root->buf);
  }
  root->buf=(unsigned char *)malloc(n + 1);
  if (oldRoot) {
    memcpy(root->buf, oldRoot, root->len);
    free(oldRoot);
  }
  memcpy(&root->buf[root->len], appendix->buf, appendix->len);
  ((char *)root->buf)[n] = 0;
  root->len=n;
}

void stringPartBytes(const IndraEnt *source, IndraEnt **ppPart, unsigned int start, unsigned long len) {
  if (ppPart == NULL) return;
  if (source == NULL) return;
  if (start > source->len) return;
  unsigned int n = len;
  if (start + n > source->len) n = source->len - start;
  *ppPart = (IndraEnt *)malloc(n+1);
  (*ppPart)->type = IT_STRING;
  (*ppPart)->len = n;
  (*ppPart)->buf = malloc(n+1);
  memcpy((*ppPart)->buf, &source->buf[start], n);
}

void stringStartBytes(const IndraEnt *source, IndraEnt **ppStart, unsigned long len) {
  stringPartBytes(source, ppStart, 0, len);
}

void stringEndBytes(const IndraEnt *source, IndraEnt **ppEnd, unsigned long len) {
  if (source == NULL) return;
  unsigned long start;
  if (source->len < len) start = 0;
  else start = source->len - len;
  stringPartBytes(source, ppEnd, start, len);
}

// XXXXX

bool stringContainsBytes(const String *source, const String *token) {
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

long stringFindBytes(const String *source, const String *token, unsigned long offset) {
  if (token==NULL) return -1;
  if (source==NULL) return -1;
  unsigned long l=0;
  long fnd = -1;
  for (unsigned long s=offset; s<source->len; s++) {
    if (l == token->len) return fnd;
    if (source->buf[s] != token->buf[l]) {
      l = 0;
      fnd = -1;
      continue;
    }
    if (fnd == -1) fnd=s;
    l += 1;
  }
  if (l >= token->len) return fnd;
  else return -1;
}

long stringFindCountBytes(const String *source, const String *token) {
  if (token==NULL) return -1;
  if (source==NULL) return -1;
  unsigned int l=0;
  unsigned long cnt=0;
  long fnd = -1;
  for (unsigned long s=0; s<source->len; s++) {
    if (l == token->len) {
      cnt += 1;
      printf("Found at %ld: %ld\n", fnd, cnt);
      l = 0;
      if (source->buf[s] == token->buf[l]) {
        fnd = s;
        l += 1;
        continue;
      } else {
        fnd = -1;
        continue;
      }
    }
    if (source->buf[s] != token->buf[l]) {
      l = 0;
      fnd = -1;
      continue;
    }
    if (fnd == -1) fnd = s;
    l += 1;
  }
  if (l >= token->len) {
    cnt += 1;
    printf("Final found at %ld end: %ld\n", fnd, cnt);
  }
  return cnt;
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

int stringFindUtf8(const String *source, const String *token) {
  if (source==NULL || token==NULL) return -1;
  if (source->len==0 || token->len==0) return -1;
  if (source->len<token->len) return -1;
  unsigned int ti=0, len, n=0;
  int fnd = -1;
  for (unsigned int i=0; i < source->len; i++) {
    len = utf8CharLen(source->buf[i]);
    if (len==0) return -1;
    if (token->len - ti >= len && !memcmp(&source->buf[i],&token->buf[ti],len)) {
      if (fnd == -1) fnd=n;
      ti += len;
      if (ti == token->len) return fnd;
    } else {
      fnd = -1;
      ti = 0;
    }
    i += len-1;
    n += 1;
  }
  return -1;
}

void stringPartUtf8(const String *source, String *part, unsigned int start, unsigned int len) {
  if (part == NULL) return;
  stringFree(part);
  if (source== NULL) return;
  if (stringLenUtf8(source) <= start) return;
  unsigned int clen;
  unsigned int n=0;
  int p0=-1, p1=-1;
  for (unsigned int i=0; i<source->len; i++) {
    clen = utf8CharLen(source->buf[i]);
    if (n==start) p0=i;
    if (n==start+len) {
      p1=i;
      break;
    }
    n += 1;
    i += clen-1;
  }
  if (p1==-1) p1=source->len;
  if (p0==-1) return;
  if (p0<p1) {
    part->buf = (unsigned char *)malloc(p1-p0);
    part->len = p1-p0;
    memcpy(part->buf, &source->buf[p0], p1-p0);
  }
}

// --- Just for debug:
void _toHex(char *target, unsigned char byte) {
  int hn=(byte & 0xf0) >> 4;
  int ln=(byte & 0x0f);
  *target='0'; target++;
  *target='x'; target++;
  if (hn<10) *target='0'+hn;
  else *target='A'+hn-10;
  ++target;
  if (ln<10) *target='0'+ln;
  else *target='A'+ln-10;
  ++target;
  *target=0;
}

void stringDisplayHex(const String *source) {
  if (source==NULL) return;
  if (source->len == 0) {
    printf("<empty string>\n");
    return;
  }
  unsigned int maxBytesLine = 20;
  unsigned int curBytesLine = 0;
  char l1[20*5+5*4+1];
  char l2[20*5*4+1];    // Box chars
  char charStr[5];
  
  *l1=0;
  *l2=0;
  printf("\n");
  for (unsigned int i=0; i<source->len; i++) {
    unsigned int len=utf8CharLen(source->buf[i]);
    if (len == 0 || len > 4) {
      _toHex(charStr, source->buf[i]);
      strcat(l1, " "); strcat(l1, charStr);
      strcat(l2, "└ERR┘");
    } else {
      for (unsigned int j=0; j<len; j++) {
        _toHex(charStr, source->buf[i+j]);
        strcat(l1, " "); strcat(l1, charStr);
      }
      strncpy(charStr, (char *)&source->buf[i], len);
      charStr[len]=0;
      int chrs=len*5-3;
      int n1=chrs/2-1;
      int n2=chrs-n1-2;
      strcat(l2, "└");
      for (unsigned int j=0; j<n1; j++) strcat(l2, "─");
      strcat(l2, " ");
      strcat(l2, charStr);
      strcat(l2, " ");
      for (unsigned int j=0; j<n2; j++) strcat(l2, "─");
      strcat(l2, "┘");
    }
    if (strlen(l1) > 5*20-35) {
      printf("%s\n",l1);
      printf("%s\n",l2);
      *l1=0;
      *l2=0;
    }
    i+=len-1;
  }
  if (strlen(l1)>0) {
    printf("%s\n",l1);
    printf("%s\n",l2);
  }  
}

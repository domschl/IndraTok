/*! \brief IndraTok: fast language agnostic tokenizer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_atom.h"
#include "indra_tok.h"

bool iaStringContains(IA_T_ATOM *source, IA_T_ATOM *token) {
  if (token==NULL) return false;
  if (source==NULL) return false;
  if (source->type != IA_ID_CHAR || token->type != IA_ID_CHAR) return false;
  unsigned int l=0;
  unsigned char *src = iaGetDataPtr(source);
  unsigned char *tok = iaGetDataPtr(token);
  for (unsigned int s=0; s<source->count; s++) {
    if (l+1 > token->count) return true;
    if (src[s] != tok[l]) continue;
    else l += 1;
  }
  if (l+1 > token->count) return true;
  else return false;
}

long iaStringIndexFind(IA_T_ATOM *source, IA_T_ATOM *token, size_t offset) {
  if (token==NULL) return -1;
  if (source==NULL) return -1;
  if (source->type != IA_ID_CHAR || token->type != IA_ID_CHAR) return false;
  unsigned long l=0;
  long fnd = -1;
  unsigned char *src = iaGetDataPtr(source);
  unsigned char *tok = iaGetDataPtr(token);
  for (unsigned long s=offset; s<source->count; s++) {
    if (l == token->count) return fnd;
    if (src[s] != tok[l]) {
      l = 0;
      fnd = -1;
      continue;
    }
    if (fnd == -1) fnd=s;
    l += 1;
  }
  if (l >= token->count) return fnd;
  else return -1;
}

size_t iaStringCountToken(IA_T_ATOM *source, IA_T_ATOM *token) {
  if (token==NULL) return -1;
  if (source==NULL) return -1;
  if (source->type != IA_ID_CHAR || token->type != IA_ID_CHAR) return false;
  unsigned int l=0;
  unsigned long cnt=0;
  long fnd = -1;
  unsigned char *src = iaGetDataPtr(source);
  unsigned char *tok = iaGetDataPtr(token);
  for (unsigned long s=0; s<source->count; s++) {
    if (l == token->count) {
      cnt += 1;
      printf("Found at %ld: %ld\n", fnd, cnt);
      l = 0;
      if (src[s] == tok[l]) {
        fnd = s;
        l += 1;
        continue;
      } else {
        fnd = -1;
        continue;
      }
    }
    if (src[s] != tok[l]) {
      l = 0;
      fnd = -1;
      continue;
    }
    if (fnd == -1) fnd = s;
    l += 1;
  }
  if (l >= token->count) {
    cnt += 1;
    printf("Final found at %ld end: %ld\n", fnd, cnt);
  }
  return cnt;
}

size_t iaCharUtf8Length(unsigned char ctok) {
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

bool iaStringValidateUtf8(IA_T_ATOM *source) {
  if (source==NULL) return false;
  if (source->type != IA_ID_CHAR) return false;
  size_t charLen, j;
  unsigned char *src = iaGetDataPtr(source);
  for (size_t i=0; i<source->count; i++) {
    charLen = iaCharUtf8Length(src[i]);
    if (!charLen) return false;
    if (charLen==1) continue;
    if (i+charLen > source->count) return false;
    for (j=1; j<charLen; j++) {
      if ((src[i+j] & 0xC0) != 0x80) return false;
    }
    i += charLen-1;
  }
  return true;
}

size_t iaStringUtf8Length(IA_T_ATOM *source) {
  if (source==NULL) return 0;
  if (source->type != IA_ID_CHAR) return 0;
  size_t n=0, charLen;
  unsigned char *src = iaGetDataPtr(source);
  for (size_t i=0; i<source->count; i++) {
    charLen = iaCharUtf8Length(src[i]);
    if (charLen==0) return 0;
    n+=1;
    i+=charLen-1;
  }
  return n;
}
/*
int stringFindUtf8(const IndraAtom *source, const IndraAtom *token) {
  if (source==NULL || token==NULL) return -1;
  if (source->count==0 || token->count==0) return -1;
  if (source->type != IA_CHAR || token->type != IA_CHAR) return -1;
  if (source->count<token->count) return -1;
  unsigned int ti=0, len, n=0;
  int fnd = -1;
  for (unsigned int i=0; i < source->count; i++) {
    len = utf8CharLen(((unsigned char *)source->buf)[i]);
    if (len==0) return -1;
    if (token->count - ti >= len && !memcmp(&(((unsigned char *)source->buf)[i]),&(((unsigned char *)token->buf)[ti]),len)) {
      if (fnd == -1) fnd=n;
      ti += len;
      if (ti == token->count) return fnd;
    } else {
      fnd = -1;
      ti = 0;
    }
    i += len-1;
    n += 1;
  }
  return -1;
}

long stringFindCountUtf8(const IndraAtom *source, const IndraAtom *token) {
  if (token==NULL) return -1;
  if (source==NULL) return -1;
  if (source->type != IA_CHAR || token->type != IA_CHAR) return false;
  unsigned int l=0;
  unsigned long cnt=0;
  long fnd = -1;
  for (unsigned long s=0; s<source->count; s++) {
    if (l == token->count) {
      cnt += 1;
      // printf("Found at %ld: %ld\n", fnd, cnt);
      l = 0;
      if (((unsigned char *)source->buf)[s] == ((unsigned char *)token->buf)[l]) {
        fnd = s;
        l += 1;
        continue;
      } else {
        fnd = -1;
        continue;
      }
    }
    if (((unsigned char *)source->buf)[s] != ((unsigned char *)token->buf)[l]) {
      l = 0;
      fnd = -1;
      continue;
    }
    if (fnd == -1) fnd = s;
    l += 1;
  }
  if (l >= token->count) {
    cnt += 1;
    // printf("Final found at %ld end: %ld\n", fnd, cnt);
  }
  return cnt;
}
*/

bool iaStringPartUtf8(IA_T_ATOM *source, IA_T_ATOM *pdest, size_t start, size_t len) {
  if (source== NULL) return false;
  if (source->type != IA_ID_CHAR) return false;
  if (iaStringUtf8Length(source) <= start) {
    return iaSetString(pdest, "");
  }
  size_t clen;
  size_t n=0;
  long p0=-1, p1=-1;
  for (size_t i=0; i<source->count; i++) {
    uint8_t *pc = iaGetIndexPtr(source, i);
    clen = iaCharUtf8Length(*pc);
    if (n==start) p0=i;
    if (n==start+len) {
      p1=i;
      break;
    }
    n += 1;
    i += clen-1;
  }
  if (p1==-1) p1=source->count;
  if (p0==-1) {
    return iaSetString(pdest, "");
  }
  size_t nlen = p1-p0;
  uint8_t *psrc = iaGetIndexPtr(source, p0);
  return iaCreate(pdest, IA_ID_CHAR, sizeof(uint8_t), nlen, psrc);
}

/*
IndraAtom *stringSplitUtf8(const IndraAtom *source, const IndraAtom *token) {
  if (token==NULL || *(char *)(token->buf)==0) return NULL;
  if (source==NULL) return NULL;
  if (source->type != IA_CHAR || token->type != IA_CHAR) return NULL;

  bool insertEmpty = true;
  IndraAtom *pParts = NULL;
  unsigned int l=0;
  unsigned long cnt=0;
  long fnd = -1;
  unsigned long part_start = 0;
  for (unsigned long s=0; s<source->count; s++) {
    if (l == token->count) {
      if (fnd - part_start > 0 || insertEmpty) {
        IndraAtom *prt = iaSlice(source, part_start, fnd-part_start);
        //printf("tok1: >"); iaPrint(prt); printf("<\n");
        if (pParts == NULL) pParts = iaCreate(IA_ATOM, prt, 1, 4);
        else iaJoin(&pParts, prt);
        iaDelete(prt);
      }
      cnt += 1;
      l = 0;
      if (((unsigned char *)source->buf)[s] == ((unsigned char *)token->buf)[l]) {
        part_start = s;
        fnd = s;
        l += 1;
        continue;
      } else {
        part_start = s;
        fnd = -1;
        continue;
      }
    }
    if (((unsigned char *)source->buf)[s] != ((unsigned char *)token->buf)[l]) {
      l = 0;
      if (fnd> -1) part_start = fnd;
      fnd = -1;
      continue;
    }
    if (fnd == -1) {
      fnd = s;
      //part_start = s;
    }
    l += 1;
  }
  if (l >= token->count) {
    if (fnd - part_start > 0 || insertEmpty) {
      IndraAtom *prt = iaSlice(source, part_start, fnd-part_start);
      //printf("tok2: >"); iaPrint(prt); printf("<\n");
      if (pParts == NULL) pParts = iaCreate(IA_ATOM, prt, 1, 1);
      else iaJoin(&pParts, prt);
      iaDelete(prt);
    }
    cnt += 1;
    //printf("Final found at %ld end: %ld\n", fnd, cnt);
  } else {
    if (fnd - part_start > 0 || insertEmpty) {
      IndraAtom *prt = iaSlice(source, part_start, source->count-part_start);
      //printf("tok3: >"); iaPrint(prt); printf("<\n");
      if (pParts == NULL) pParts = iaCreate(IA_ATOM, prt, 1, 1);
      else iaJoin(&pParts, prt);
      iaDelete(prt);
    }
  }
  //printf("Split-count: %lu\n", pParts->count);
  return pParts;
}
*/

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

void iaStringDisplayHex(IA_T_ATOM *source) {
  if (source==NULL) return;
  if (source->type != IA_ID_CHAR) return;
  if (source->count == 0) {
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
  for (unsigned int i=0; i<source->count; i++) {
    char *pc = iaGetIndexPtr(source, i);
    unsigned int len=iaCharUtf8Length(*pc);
    if (len == 0 || len > 4) {
      _toHex(charStr, (unsigned char)*pc);
      strcat(l1, " "); strcat(l1, charStr);
      strcat(l2, "└ERR┘");
    } else {
      for (unsigned int j=0; j<len; j++) {
        char *pcj = iaGetIndexPtr(source, i+j);
        _toHex(charStr, (unsigned char) *pcj);
        strcat(l1, " "); strcat(l1, charStr);
      }
      strncpy(charStr, pc, len);
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

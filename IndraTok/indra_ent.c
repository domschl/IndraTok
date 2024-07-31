#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indra_ent.h"


void itDelete(IndraEnt *pie) {
  if (pie == NULL) return;
  if (pie->type != IT_NIL && pie->len>0 && pie->buf != NULL) {
    free(pie->buf);
  }
  free(pie);
}

IndraEnt *itCreateChar(char c) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(char));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(char *)pie->buf=c;
  pie->type = IT_CHAR;
  pie->len = sizeof(char);
  return pie;
}

IndraEnt *itCreateByte(unsigned char byte) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned char));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned char *)pie->buf=byte;
  pie->type = IT_BYTE;
  pie->len = sizeof(unsigned char);
  return pie;
}

IndraEnt *itCreateBytes(unsigned char bytes[], unsigned long len) {
  if (bytes == NULL) return NULL;
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned char) * len);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memcpy(pie->buf, bytes, sizeof(unsigned char) * len);
  pie->type = IT_BYTES;
  pie->len = sizeof(unsigned char) * len;
  return pie;
}

IndraEnt *itCreateString(char str[]) {
  if (str == NULL) return NULL;
  unsigned long len = strlen(str);
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(char) * len + 1);
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  memcpy(pie->buf, str, sizeof(char) * len + 1);
  pie->type = IT_STRING;
  pie->len = sizeof(char) * len;
  return pie;
}

IndraEnt *itCreateUtf8(char strUtf8[]) {
  if (strUtf8 == NULL) return NULL;
  IndraEnt *pie = itCreateString(strUtf8);
  if (pie == NULL) return NULL;
  pie->type = IT_UTF8;
  return pie;
}

IndraEnt *itCreateInt(int i) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(int));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(int *)pie->buf=i;
  pie->type = IT_INT;
  pie->len = sizeof(int);
  return pie;
}

IndraEnt *itCreateUInt(unsigned int ui) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned int));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned int *)pie->buf=ui;
  pie->type = IT_UINT;
  pie->len = sizeof(unsigned int);
  return pie;
}

IndraEnt *itCreateLong(long l) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(long));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(long *)pie->buf=l;
  pie->type = IT_LONG;
  pie->len = sizeof(long);
  return pie;
}

IndraEnt *itCreateULong(unsigned long ul) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(unsigned long));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(unsigned long *)pie->buf=ul;
  pie->type = IT_ULONG;
  pie->len = sizeof(unsigned long);
  return pie;
}

IndraEnt *itCreateFloat(float f) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(float));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(float *)pie->buf=f;
  pie->type = IT_FLOAT;
  pie->len = sizeof(float);
  return pie;
}

IndraEnt *itCreateDouble(double df) {
  IndraEnt *pie = (IndraEnt *)malloc(sizeof(IndraEnt));
  if (!pie) return NULL;
  memset(pie, 0, sizeof(IndraEnt));
  pie->buf=malloc(sizeof(double));
  if (!pie->buf) {
    free(pie);
    return NULL;
  }
  *(double *)pie->buf=df;
  pie->type = IT_DOUBLE;
  pie->len = sizeof(double);
  return pie;
}

void itPrint(IndraEnt *pie) {
  if (pie == NULL) {
    printf("<NULL>");
    return;
  }
  char buf[10];
  switch (pie->type) {
  case IT_NIL:
    printf("<NIL>");
    break;
  case IT_CHAR:
    printf("%c", *(char *)pie->buf);
    break;
  case IT_BYTE:
    printf("0x%02x", *(unsigned char *)pie->buf);
    break;
  case IT_BYTES:
    printf("0x0000  ");
    for (unsigned long i=0; i<pie->len; i++) {
      printf("0x%02x ", ((unsigned char *)pie->buf)[i]);
      if ((i+1)%8 == 0) printf("\n0x%04x  ", (unsigned int)(i%0x10000));
    }
    printf("\n");
    break;
  case IT_STRING:
    printf("%s", (char *)pie->buf);
    break;
  case IT_UTF8:
    printf("%s", (char *)pie->buf);
    break;
  case IT_INT:
    printf("%d", *(int *)pie->buf);
    break;
  case IT_UINT:
    printf("%u", *(unsigned int *)pie->buf);
    break;
  case IT_LONG:
    printf("%ld", *(long *)pie->buf);
    break;
  case IT_ULONG:
    printf("%lu", *(unsigned long *)pie->buf);
    break;
  case IT_FLOAT:
    printf("%f", *(float *)pie->buf);
    break;
  case IT_DOUBLE:
    printf("%lf", *(double *)pie->buf);
    break;
  case IT_INFNUM:
    printf("<NOT_IMPL>");
    break;
  }
}

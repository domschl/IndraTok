#pragma once

#include <stdlib.h>
#include <string.h>

typedef enum _indra_types {NIL=0, CHAR, BYTE, BYTES, STRING, UTF8, INT, UINT, LONG, ULONG, FLOAT, DOUBLE, INFNUM} IndraTypes;

typedef struct _indra_ent {
  void *buf;
  IndraTypes type;
  unsigned long len;
} IndraEnt;

typedef struct _indra_ent_array {
  void *buf;
  IndraTypes type;
  unsigned long elementLen;
  unsigned long count;
} IndraEntArray;

void itDelete(IndraEnt *pie) {
  if (pie == NULL) return;
  if (pie->type != NIL && pie->len>0 && pie->buf != NULL) {
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
  pie->type = CHAR;
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
  pie->type = BYTE;
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
  pie->type = BYTES;
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
  pie->type = STRING;
  pie->len = sizeof(char) * len;
  return pie;
}

IndraEnt *itCreateUtf8(char strUtf8[]) {
  if (strUtf8 == NULL) return NULL;
  IndraEnt *pie = itCreateString(strUtf8);
  if (pie == NULL) return NULL;
  pie->type = UTF8;
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
  pie->type = INT;
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
  pie->type = UINT;
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
  pie->type = LONG;
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
  pie->type = ULONG;
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
  pie->type = FLOAT;
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
  pie->type = DOUBLE;
  pie->len = sizeof(double);
  return pie;
}





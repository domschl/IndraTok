#pragma once
#include <stdbool.h>

typedef struct _string {
  unsigned int len;
  unsigned char *buf;
} String;

void stringFree(String *string);
void stringFromCharString(String *string, const char* charString);
char* stringToCharStringAlloc(const String *string);
void stringToCharStringN(const String *string, char *charString, unsigned int charStringMemSize);
void stringAppend(String *root, const String *appendix);
void stringPartByte(const String *source, String *part, unsigned int start, unsigned int len);
void stringStartByte(const String *source, String *start, unsigned int len);
void stringEndByte(const String *source, String *end, unsigned int len);
bool stringContains(const String *source, const String *token);
void stringPrint(const String *source);
void stringPrintLn(const String *source);
unsigned int utf8CharLen(unsigned char ctok);
unsigned int stringLenUtf8(const String *source);
bool stringValidateUtf8(const String *source);
void stringDisplayHex(const String *source);

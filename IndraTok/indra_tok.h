#pragma once
#include <stdbool.h>

typedef struct _string {
  unsigned char *buf;
  unsigned long len;
} String;

typedef struct _string_array {
  unsigned long len;
  String *pStrings;
} StringArray;

void stringFree(String *string);

void stringFromCharString(String *string, const char* charString);
char* stringToCharStringAlloc(const String *string);
void stringToCharStringN(const String *string, char *charString, unsigned int charStringMemSize);

void stringAppend(String *root, const String *appendix);

void stringPartBytes(const String *source, String *part, unsigned int start, unsigned long len);
void stringStartBytes(const String *source, String *start, unsigned long len);
void stringEndBytes(const String *source, String *end, unsigned long len);
bool stringContainsBytes(const String *source, const String *token);
long stringFindBytes(const String *source, const String *token, unsigned long offset);
long stringFindCountBytes(const String *source, const String *token);

void stringPrint(const String *source);
void stringPrintLn(const String *source);

bool stringValidateUtf8(const String *source);
unsigned int utf8CharLen(unsigned char ctok);
unsigned int stringLenUtf8(const String *source);
int stringFindUtf8(const String *source, const String *token);
void stringPartUtf8(const String *source, String *part, unsigned int start, unsigned int len);

// --- dbg
void stringDisplayHex(const String *source);

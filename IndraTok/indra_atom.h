//        7       6       5       4       3       2       1       0
// 7654321076543210765432107654321076543210765432107654321076543210
// |----
// 1  2

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum types {IA_ID_NIL=0, IA_ID_CHAR, IA_ID_WORD, IA_ID_INT, IA_ID_LONG, IA_ID_FLOAT,
                    IA_ID_DOUBLE, IA_ID_ATOM, IA_ID_PANY} IA_T_TYPES;

typedef struct _ia_t_heap_header {
    size_t capacity;
    size_t recsize;
    } IA_T_HEAP_HEADER;

// Stack preference: the higher the number, the more data is stored on the stack
// before using heap allocation. Default 1, can be overridden by defining
#ifndef IA_STACK_PREFERENCE
#define IA_STACK_PREFERENCE 1
#endif

typedef struct _ia_atom IA_T_ATOM;

#define IA_T_ATOM_SIZE (sizeof(unsigned long)*2)

// This defines the maximum array count of elements that are stored on the stack
// If the count is greater than this, the data is stored on the heap using the IA_SUBATOM struct
#define IA_STACK_CHARS  (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(uint8_t))
#define IA_STACK_WORDS  (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(uint16_t))
#define IA_STACK_INTS   (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(uint32_t))
#define IA_STACK_LONGS  (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(uint64_t))
#define IA_STACK_FLOATS (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(float))
#define IA_STACK_DOUBLES (sizeof(void *) *IA_STACK_PREFERENCE / sizeof(double))

const unsigned long iaStackMax[] = {0, IA_STACK_CHARS, IA_STACK_WORDS, IA_STACK_INTS, IA_STACK_LONGS, IA_STACK_FLOATS, IA_STACK_DOUBLES, 0, 0};

//* Std struct
struct _ia_atom {
  unsigned long onHeap:1;  //* is on heap
  unsigned long type:5;    //* data type
  size_t count:58;         //* 64-5-1 XXX arch-dep! count of elements, <=IA_STACK_* on stack, >IA_STACK_* on heap
  //* actual data
  union _data {
    uint8_t c[IA_STACK_CHARS];
    uint16_t w[IA_STACK_WORDS];
    uint32_t i[IA_STACK_INTS];
    uint64_t l[IA_STACK_LONGS];
    float f[IA_STACK_FLOATS];
    double d[IA_STACK_DOUBLES];
    IA_T_HEAP_HEADER *pHeap;
  } data;
};

const unsigned long iaTypesize[IA_ID_PANY+1] = {0, sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t), sizeof(float), sizeof(double), sizeof(struct _ia_atom), sizeof(void *)};

void iaDelete(IA_T_ATOM *pAtom);
bool iaCreate(IA_T_ATOM *pAtom, int type, size_t recsize, size_t count, void *pData);
void iaSetChar(IA_T_ATOM *pAtom, uint8_t value);
void iaSetWord(IA_T_ATOM *pAtom, uint16_t value);
void iaSetInt(IA_T_ATOM *pAtom, uint32_t value);
void iaSetLong(IA_T_ATOM *pAtom, uint64_t value);
void iaSetFloat(IA_T_ATOM *pAtom, float value);
void iaSetDouble(IA_T_ATOM *pAtom, double value);
void iaSetAtom(IA_T_ATOM *pAtom, IA_T_ATOM *pValue);
void iaSetPany(IA_T_ATOM *pAtom, void *pValue);
void iaSetNil(IA_T_ATOM *pAtom);

void iaPrint(IA_T_ATOM *pAtom);

bool iaSetString(IA_T_ATOM *pAtom, char *pString);
void *iaGetStackDataPtr(IA_T_ATOM *pAtom);
void *iaGetHeapDataPtr(IA_T_ATOM *pAtom);
void *iaGetDataPtr(IA_T_ATOM *pAtom);
void *iaGetIndexStackPtr(IA_T_ATOM *pAtom, size_t index);
void *iaGetIndexHeapPtr(IA_T_ATOM *pAtom, size_t index);
void *iaGetIndexPtr(IA_T_ATOM *pAtom, size_t index);
bool iaExpand(IA_T_ATOM *pAtom, size_t new_capacity);
bool iaSetIndex(IA_T_ATOM *pAtom, size_t index, void *pData);
bool iaSetIndexExpand(IA_T_ATOM *pAtom, size_t index, void *pData);
bool iaAppend(IA_T_ATOM *pAtom, void *pData);

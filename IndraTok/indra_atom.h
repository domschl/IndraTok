//        7       6       5       4       3       2       1       0
// 7654321076543210765432107654321076543210765432107654321076543210
// |----
// 1  2 
#include <stdint.h>

typedef enum types {IA_ID_NIL=0, IA_ID_CHAR, IA_ID_WORD, IA_ID_INT, IA_ID_LONG, IA_ID_FLOAT,
                    IA_ID_DOUBLE, IA_ID_ATOM, IA_ID_PANY} IA_T_TYPES;

typedef struct _ia_t_heap_header {
    uint64_t count;
    uint64_t recsize;
    } IA_T_HEAP_HEADER;

typedef struct _ia_t_heap {
  uint64_t capacity;
  uint64_t recsize;
  void *pData;
} IA_T_HEAP;

// Stack preference: the higher the number, the more data is stored on the stack
// before using heap allocation. Default 1, can be overridden by defining
#ifndef IA_STACK_PREFERENCE
#define IA_STACK_PREFERENCE 1
#endif

typedef struct _ia_atom IA_T_ATOM;

#define IA_T_ATOM_SIZE (sizeof(unsigned long)*2)

// This defines the maximum array count of elements that are stored on the stack
// If the count is greater than this, the data is stored on the heap using the IA_SUBATOM struct
#define IA_STACK_CHARS  (sizeof(uint8_t)*IA_STACK_PREFERENCE/sizeof(void *))
#define IA_STACK_WORDS  (sizeof(uint16_t)*IA_STACK_PREFERENCE/sizeof(void *))
#define IA_STACK_INTS   (sizeof(uint32_t)*IA_STACK_PREFERENCE/sizeof(void *))
#define IA_STACK_LONGS  (sizeof(uint64_t)*IA_STACK_PREFERENCE/sizeof(void *))
#define IA_STACK_FLOATS (sizeof(float)*IA_STACK_PREFERENCE/sizeof(void *))
#define IA_STACK_DOUBLES (sizeof(double)*IA_STACK_PREFERENCE/sizeof(void *))

const unsigned long iaStackMax[IA_ID_PANY+1] = {0, IA_STACK_CHARS, IA_STACK_WORDS, IA_STACK_INTS, IA_STACK_LONGS, IA_STACK_FLOATS, IA_STACK_DOUBLES, 0, 0};

//* Std struct
struct _ia_atom {
  uint64_t onHeap:1;  //* is on heap
  uint64_t type:5;    //* data type
  uint64_t count: sizeof(long)-(1+5);  //* count of elements, <=IA_STACK_* on stack, >IA_STACK_* on heap
  //* actual data
  union _data {
    uint8_t c[IA_STACK_CHARS];
    uint16_t w[IA_STACK_WORDS];
    uint32_t i[IA_STACK_INTS];
    uint64_t l[IA_STACK_LONGS];
    float f[IA_STACK_FLOATS];
    double d[IA_STACK_DOUBLES];
    IA_T_HEAP *pHeap;
  } data;
};

const unsigned long iaTypesize[IA_ID_PANY+1] = {0, sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t), sizeof(float), sizeof(double), sizeof(struct _ia_atom), sizeof(void *)};

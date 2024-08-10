//        7       6       5       4       3       2       1       0
// 7654321076543210765432107654321076543210765432107654321076543210
// |----
// 1  2 
#include <stdint.h>

typedef enum types {IA_ID_NIL=0, IA_ID_CHAR, IA_ID_WORD, IA_ID_INT, IA_ID_LONG, IA_ID_FLOAT,
                    IA_DOUBLE, IA_ID_ARRAY, IA_ID_ATOM, IA_ID_ANYPTR} IA_T_TYPES;

// Heap data struct
typedef struct _it_heap_atom {
  uint64_t type;
  uint64_t recsize;
  uint64_t count;
  uint64_t capacity;
  void *pData;
} IA_T_HEAP_ATOM;

// Stack preference: the higher the number, the more data is stored on the stack
// before using heap allocation. Default 1
#define STACK_PREFERENCE 1

// This defines the maximum array count of elements that are stored on the stack
// If the count is greater than this, the data is stored on the heap using the IA_HEAP_ATOM struct
#define IA_HEAP_ATOM_SIZE (sizeof(IA_T_HEAP_ATOM)*STACK_PREFERENCE)
#define IA_STACK_DOUBLES (IA_HEAP_ATOM_SIZE/sizeof(double)*STACK_PREFERENCE)
#define IA_STACK_FLOATS (IA_HEAP_ATOM_SIZE/sizeof(float)*STACK_PREFERENCE)
#define IA_STACK_LONGS (IA_HEAP_ATOM_SIZE/sizeof(uint64_t)*STACK_PREFERENCE)
#define IA_STACK_INTS (IA_HEAP_ATOM_SIZE/sizeof(uint32_t)*STACK_PREFERENCE)
#define IA_STACK_WORDS (IA_HEAP_ATOM_SIZE/sizeof(uint16_t)*STACK_PREFERENCE)
#define IA_STACK_CHARS (IA_HEAP_ATOM_SIZE/sizeof(uint8_t)*STACK_PREFERENCE)
#define IA_STACK_ATOMS (IA_HEAP_ATOM_SIZE/sizeof(IA_T_HEAP_ATOM)*STACK_PREFERENCE)
#define IA_STACK_ANYPTR (IA_HEAP_ATOM_SIZE/sizeof(void *)*STACK_PREFERENCE)

//* Std struct
typedef struct _it_atom {
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
    IA_T_HEAP_ATOM heap_atom[IA_STACK_ATOMS];
    void *anyPtr[IA_STACK_ANYPTR];
  } dat;
} IA_T_ATOM;


//        7       6       5       4       3       2       1       0
// 7654321076543210765432107654321076543210765432107654321076543210
// |----
// 1  2 
#include <stdint.h>

typedef enum types {NIL=0, CHAR=1, INT=2, LONG=3, FLOAT=4, DOUBLE=5, ARRAY=6, OBJECT=7} T_TYPES;

// Heap data struct
typedef struct _it_heap_atom {
  uint64_t type;
  uint64_t recsize;
  uint64_t count;
  uint64_t capacity;
  void *pData;
} IT_HEAP_ATOM;

// This defines the maximum array count of elements that are stored on the stack
// If the count is greater than this, the data is stored on the heap using the IT_HEAP_ATOM struct
#define IT_HEAP_ATOM_SIZE (sizeof(IT_HEAP_ATOM))
#define IT_STACK_DOUBLES (IT_HEAP_ATOM_SIZE/sizeof(double))
#define IT_STACK_FLOATS (IT_HEAP_ATOM_SIZE/sizeof(float))
#define IT_STACK_LONGS (IT_HEAP_ATOM_SIZE/sizeof(uint64_t))
#define IT_STACK_INTS (IT_HEAP_ATOM_SIZE/sizeof(uint32_t))
#define IT_STACK_WORDS (IT_HEAP_ATOM_SIZE/sizeof(uint16_t))
#define IT_STACK_CHARS (IT_HEAP_ATOM_SIZE/sizeof(uint8_t))

//* Std struct
typedef struct _it_atom {
  uint64_t onHeap:1;  //* is on heap
  uint64_t type:5;    //* data type
  uint64_t count: sizeof(long)-(1+5);  //* count of elements, <=IT_STACK_* on stack, >IT_STACK_* on heap
  //* actual data
  union _data {
    uint8_t c[IT_STACK_CHARS];
    uint16_t w[IT_STACK_WORDS];
    uint32_t i[IT_STACK_INTS];
    uint64_t l[IT_STACK_LONGS];
    float f[IT_STACK_FLOATS];
    double d[IT_STACK_DOUBLES];
    IT_HEAP_ATOM heap_atom;
  } dat;
} IT_ATOM;


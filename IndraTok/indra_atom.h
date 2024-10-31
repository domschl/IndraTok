/**
 * @file indra_atom.h
 * @brief Header file for IndraTok atom data structure and related functions.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>  // IWYU pragma: export. // size_t

/**
 * @enum types
 * @brief Enumeration of data types supported by IA_T_ATOM.
 */
typedef enum types {
    IA_ID_NIL = 0,   /**< Nil type */
    IA_ID_CHAR,      /**< Character type */
    IA_ID_WORD,      /**< Word type */
    IA_ID_INT,       /**< Integer type */
    IA_ID_LONG,      /**< Long integer type */
    IA_ID_FLOAT,     /**< Float type */
    IA_ID_DOUBLE,    /**< Double type */
    IA_ID_ATOM,      /**< Atom type */
    IA_ID_PANY       /**< Pointer to any type */
} IA_T_TYPES;

/**
 * @struct _ia_t_heap_header
 * @brief Header for heap-allocated data.
 */
typedef struct _ia_t_heap_header {
    size_t capacity; /**< Capacity of the heap allocation */
    size_t recsize;  /**< Size of each record */
} IA_T_HEAP_HEADER;

// Stack preference: the higher the number, the more data is stored on the stack
// before using heap allocation. Default 1, can be overridden by defining
#ifndef IA_STACK_PREFERENCE
#define IA_STACK_PREFERENCE (1)
#endif

#ifndef IA_MIN_NEW_EXPAND
#define IA_MIN_NEW_EXPAND (8)
#endif

#ifndef IA_MIN_NEW_CAPACITY
#define IA_MIN_NEW_CAPACITY (4)
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


/**
 * @struct _ia_atom
 * @brief Structure representing an atom.
 */
struct _ia_atom {
    unsigned long onHeap : 1; /**< Flag indicating if data is on heap */
    unsigned long type : 5;   /**< Data type */
    size_t count : 58;        /**< Count of elements, 58 bits, together with type (5 bits) and onHeap (1 bit) 64 bits are used */
    union _data {
        uint8_t  c[IA_STACK_CHARS];      /**< Character data */
        uint16_t w[IA_STACK_WORDS];      /**< Word data */
        uint32_t i[IA_STACK_INTS];       /**< Integer data */
        uint64_t l[IA_STACK_LONGS];      /**< Long integer data */
        float    f[IA_STACK_FLOATS];     /**< Float data */
        double   d[IA_STACK_DOUBLES];    /**< Double data */
        IA_T_HEAP_HEADER *pHeap;         /**< Pointer to heap data */
    } data; /**< Union for storing data */
};

void iaDelete(IA_T_ATOM *pAtom);
bool iaCreateCapacity(IA_T_ATOM *pAtom, int type, size_t recsize, size_t capacity, size_t count, void *pData);
bool iaCreate(IA_T_ATOM *pAtom, int type, size_t recsize, size_t count, void *pData);
size_t iaGetRecsize(const IA_T_ATOM *pAtom);
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
void iaPrintLn(IA_T_ATOM *pAtom);

bool iaSetString(IA_T_ATOM *pAtom, char *pString);
void *iaGetStackDataPtr(IA_T_ATOM *pAtom);
void *iaGetHeapDataPtr(IA_T_ATOM *pAtom);
void *iaGetDataPtr(IA_T_ATOM *pAtom);
void *iaGetIndexStackPtr(IA_T_ATOM *pAtom, size_t index);
void *iaGetIndexHeapPtr(IA_T_ATOM *pAtom, size_t index);
void *iaGetIndexPtr(IA_T_ATOM *pAtom, size_t index);
void *iaGetIndexPtr2D(IA_T_ATOM *patom, size_t index1, size_t index2, bool verbose);
void *iaGetIndexPtr3D(IA_T_ATOM *patom, size_t index1, size_t index2, size_t index3, bool verbose);
bool iaExpand(IA_T_ATOM *pAtom, size_t new_capacity);
bool iaSetIndex(IA_T_ATOM *pAtom, size_t index, void *pData);
bool iaSetIndexExpand(IA_T_ATOM *pAtom, size_t index, void *pData);
bool iaAppend(IA_T_ATOM *pAtom, void *pData);
bool iaCopy(IA_T_ATOM *pSrc, IA_T_ATOM *pDest);
bool iaJoin(IA_T_ATOM *pRoot, IA_T_ATOM *pAppend);
bool iaSlice(IA_T_ATOM *pSrc, IA_T_ATOM *pDest, size_t start, size_t len);

/**
 * @struct _ia_t_map
 * @brief Structure representing a map.
 */
typedef struct _ia_t_map {
    unsigned long fillLevel; /**< Fill level of the map */
    IA_T_ATOM hash;          /**< Hash atom */
    IA_T_ATOM values;        /**< Values atom */
} IA_T_MAP;

bool iaCreateMap(IA_T_MAP *pMap, size_t capacity);
bool iaMapGet(IA_T_MAP *pMap, IA_T_ATOM *pKey, IA_T_ATOM *pValue);
bool iaMapSet(IA_T_MAP *pMap, IA_T_ATOM *pKey, IA_T_ATOM *pValue);
void iaMapDelete(IA_T_MAP *pMap);
bool iaMapRemove(IA_T_MAP *pMap, IA_T_ATOM *pKey);

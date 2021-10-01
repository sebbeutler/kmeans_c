#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef char byte_t;

// TODO: Buffer before the start of the data array for faster top insertions

#define VEC(vec, type, id) ((type*)(vec.start))[(id)]

/*
    Sequence of bytes that can dynamically grow and shrink in size.
    In the form of a dynamic array.
    Extra space can be allocated to add element faster.
    Access is done by copying value of given pointers to the dynamic array.

    Note: the type used should be align with a memory word for proper usage.
*/
typedef struct vector vector;

struct vector
{
    size_t type_size;

    uint32_t count;
    uint32_t max;

    byte_t* start;
    byte_t* finish;
    byte_t* end_storage;
};

/*
    Create a new vector.
    Set 'cacheCount' to 0 for no extra space in the dynamic array.
*/
vector new_vector(size_t type_size, uint32_t elementCount, uint32_t cacheCount);

/*
    Copy the value of 'val' at the end of the vector.
*/
void vec_push(vector* vec, void* val);

/*
    Remove the last element of the vector and allocate a copy referenced by 'dest'.
    Returns NULL if the vector is empty.
*/
void vec_pop(vector* vec, void** dest);

/*
    Copy the data pointed by 'val' to a given index inside the vector.
    Cannot insert outside the range of [start,finish] (index should be < vec.count).
*/
void vec_set(vector* vec, void* val, uint32_t index);

/*
    Return a pointer to a position in the dynamic array.
*/
void* vec_get(vector* vec, uint32_t index);

/*
    Resize the dynamic array to a given number of element.
*/
void vec_resize(vector* vec, uint32_t elementCount);

/*
    Free the total space used by the vector.
*/
void free_vector(vector* vec);

#endif // !VECTOR_H

#include "data_structures/vector.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

vector new_vector(size_t type_size, uint32_t elementCount, uint32_t cacheCount)
{
    vector new_vector;

    new_vector.type_size = type_size;
    new_vector.count = elementCount;

    if (cacheCount)
    {
        new_vector.max = elementCount + cacheCount;

        new_vector.start = malloc((elementCount + cacheCount) * type_size);
        new_vector.finish = new_vector.start + elementCount * type_size;

        new_vector.end_storage = new_vector.start + (elementCount + cacheCount) * type_size;
    }
    else
    {
        new_vector.max = elementCount;

        new_vector.start = malloc(elementCount * type_size);
        new_vector.finish = new_vector.start + type_size * elementCount;

        new_vector.end_storage = new_vector.finish;
    }

    return new_vector;
}

void vec_push(vector* vec, void* val)
{
    vec->count++;
    if (vec->max < vec->count)
        vec_resize(vec, vec->count);
    memcpy(vec->finish, val, vec->type_size);
    vec->finish = vec->finish + vec->type_size;
    vec->count++;
}

void vec_pop(vector* vec, void** dest)
{
    if (vec->start == vec->finish) // empty vector
    {
        *dest = NULL;
        return;
    }

    *dest = malloc(vec->type_size);
    if (*dest == NULL) return;

    vec->finish = vec->finish - vec->type_size;
    memcpy(*dest, vec->finish, vec->type_size);
    vec->count--;
}

void vec_set(vector* vec, void* val, uint32_t index)
{
    if (index >= vec->count)
        return;
    memcpy(vec->start + vec->type_size * index, val, vec->type_size);
}

void* vec_get(vector* vec, uint32_t index)
{
    return vec->start + vec->type_size * index;
}

void vec_resize(vector* vec, uint32_t maxCount)
{
    if (vec->max == maxCount)
        return;

    vec->max = maxCount;
    if (vec->count > maxCount)
    {
        vec->count = maxCount;
    }

    byte_t* new_mem = realloc(vec->start, vec->type_size * maxCount);
    if (new_mem == NULL) return;

    vec->start = new_mem;
    vec->finish = new_mem + vec->count * vec->type_size;
    vec->end_storage = new_mem + maxCount * vec->type_size;
}

void free_vector(vector* vec)
{
    if (vec == NULL)
        return;

    free(vec->start);
}
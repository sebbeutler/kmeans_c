#ifndef POOL_H
#define POOL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct clist clist;
typedef clist pool;

/*
Returns a memory block of the given size from the pool.
Allocates a new block if none are avaiable.
*/
void* request(pool** _pool, size_t size);

/*
Save unused memory blocks into the pool for futur uses.
If a type is given, all bits int the block will be set to 0.
*/
void pfree(pool** _pool, void** data);

#endif
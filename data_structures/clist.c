#include "data_structures/clist.h"

#include "pcg_basic.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void* cy_insert(clist** node, void* data)
{
	clist* new_node = malloc(sizeof(clist));
	if (new_node == NULL) return NULL;

	if (*node == NULL) // empty list
	{
		*node = new_node;
		(*node)->data = data;
		(*node)->next = *node;
	}
	else
	{
		new_node->data = data;
		new_node->next = (*node)->next;

		(*node)->next = new_node;
	}

	return data; // SUCCESS
}

void cy_clear(clist** node)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

void cy_clean(clist** node)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

void cy_pclean(clist** node, clist** pool)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		cy_insert(pool, current->data);
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

unsigned int cy_len(clist* node)
{
	if (node == NULL)
		return 0;

	uint32_t i = 0;
	CY_ITER(node, iter,
		i++;
	);
	return i;
}

void* cy_random_data(clist* node)
{
	if (node == NULL)
		return NULL;

	uint32_t index = pcg32_boundedrand(cy_len(node));

	uint32_t i = 0;
	CY_ITER(node, iter,
		if (index == i)
			return iter->data;
	i++;
	);

	return NULL;
}

void* cy_random_max(clist* node, uint32_t max)
{
	if (node == NULL)
		return NULL;

	uint32_t index = pcg32_boundedrand(max);

	uint32_t i = 0;
	CY_ITER(node, iter,
		if (index == i)
			return iter->data;
	i++;
	);

	return NULL;
}

clist* cy_find(clist* node, void* data)
{
	if (node == NULL)
		return NULL;

	CY_ITER(node, iter,
		if (iter->data == data)
			return iter;
	);

	return NULL;
}

bool cy_remove( clist** node, void* data )
{
	if ( *node == NULL )
		return false;
	
	if ( (*node)->next == *node )
	{
		if ( (*node)->data == data )
		{
			free(*node);
			*node = NULL;
			return true;
		}
		return false;
	}

	if ( (*node)->data == data )
	{
		clist* nxt_node = (*node)->next; 
		(*node)->next = nxt_node->next;
		(*node)->data = nxt_node->data;
		free(nxt_node);
		return true;
	}

	clist* iter = *node;
	clist* previous = iter;
	do
	{
		if ( iter->data == data )
		{
			previous->next = iter->next;
			free( iter );
			return true;
		}
		previous = iter;
		iter = iter->next;
	} while ( iter != *node );
	return false;
}

clist* cy_clone(clist* node)
{
    clist* clone = NULL;

    CY_ITER_DATA(node, iter, data, void*,
        cy_insert(&clone, data);
    );

    return clone;
}
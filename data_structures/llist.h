#pragma once
#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct llist llist;

struct llist
{
	void* data;
	llist* next;
};

void llpush(llist** list, void* data_ptr);

void llappend(llist* list, void* data_ptr);

void lldestroy_list(llist** head);

#endif
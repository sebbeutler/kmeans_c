#ifndef KMEANS_H
#define KMEANS_H

#include "data_structures/clist.h"

typedef struct Point Point;

void kmeans_init();

void kmeans_step();

void kmeans_end();

int speciate( clist* pointlist, clist* species );

double distance_point( Point *p1, Point *p2 );

void kmeans();

#endif // !KMEANS_H
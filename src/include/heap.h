#include <stdint.h>
#include "vector.h"

#ifndef KODEX_HEAP
#define KODEX_HEAP

typedef struct heap
{
    vector_t * vec;
    int8_t (* cmp)(void *, void *);
} heap_t;

heap_t * heap_new();
heap_t * heap_destroy(heap_t * heap);
void heap_add(heap_t * heap, void * element);
void * heap_get(heap_t * heap);
void * heap_remove(heap_t * heap);

#endif
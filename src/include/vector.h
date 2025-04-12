#include <stddef.h>

#ifndef KODEX_VECTOR
#define KODEX_VECTOR

typedef struct vector
{
    size_t capacity;
    size_t size;
    void ** elements;
} vector_t;

vector_t * vector_new(size_t capacity);
vector_t * vector_destroy(vector_t * vector);
void vector_add(vector_t * vector, void * element);
void * vector_get(vector_t * vector, size_t index);
void * vector_set(vector_t * vector, size_t index, void * element);
void * vector_remove(vector_t * vector, size_t index);

#endif
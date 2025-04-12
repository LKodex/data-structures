#include "vector.h"
#include <stdlib.h>

/**
 * creates a new vector with at least 1 element capacity
 */
vector_t * vector_new(size_t capacity)
{
    vector_t * vector = malloc(sizeof(vector_t));
    if (capacity <= 0)
    {
        capacity = 1;
    }
    vector->capacity = capacity;
    vector->size = 0;
    vector->elements = calloc(capacity, sizeof(void *));
    return vector;
}

/**
 * free the struct vector_t
 * free its void ** elements
 * free every element remaining
 * @returns NULL
 */
vector_t * vector_destroy(vector_t * vector)
{
    if (vector == NULL)
    {
        return NULL;
    }
    
    for (size_t i = 0; i < vector->size; i++)
    {
        free(vector->elements[i]);
    }
    free(vector->elements);
    vector->elements = NULL;
    free(vector);
    vector = NULL;
    return vector;
}

/**
 * adds the element at the end of the vector.
 * if the vector size reached the vector capacity it doubles de capacity
 */
void vector_add(vector_t * vector, void * element)
{
    if (vector->size >= vector->capacity)
    {
        vector->capacity *= 2;
        vector->elements = realloc(vector->elements, sizeof(void *) * vector->capacity);
    }
    vector->elements[vector->size++] = element;
}

/**
 * @returns the element at the index. NULL if the index does not exist
 */
void * vector_get(vector_t * vector, size_t index)
{
    if (index >= vector->size)
    {
        return NULL;
    }
    return vector->elements[index];
}

/**
 * @returns the replaced element. NULL if no element was replaced (it stills append the element at the end of the vector)
 */
void * vector_set(vector_t * vector, size_t index, void * element)
{
    if (index >= vector->size)
    {
        vector_add(vector, element);
        return NULL;
    }
    void * old = vector->elements[index];
    vector->elements[index] = element;
    return old;
}

/**
 * @returns the removed element. NULL if no element was removed
 */
void * vector_remove(vector_t * vector, size_t index)
{
    if (index >= vector->size)
    {
        return NULL;
    }
    void * removed = vector->elements[index];
    for (size_t i = index; i < vector->size - 1; i++)
    {
        vector->elements[i] = vector->elements[i + 1];
    }
    vector->elements[--vector->size] = NULL;
    return removed;
}

#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

int main()
{
    vector_t * vec = vector_new(10);
    for (size_t i = 0; i < 10; i++)
    {
        size_t * e = (size_t *) malloc(sizeof(size_t));
        *e = i;
        vector_add(vec, e);
    }

    for (size_t i = 0; i < vec->size; i++)
    {
        printf("%ld\n", * (size_t *) vector_get(vec, i));
    }

    vec = vector_destroy(vec);
    
    return EXIT_SUCCESS;
}

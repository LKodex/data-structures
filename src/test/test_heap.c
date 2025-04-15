#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

int8_t fn_int_cmp(void * left, void * right)
{
    int8_t result = 0;
    int * left_int = (int *) malloc(sizeof(int));
    int * right_int = (int *) malloc(sizeof(int));
    *left_int = INT_MIN;
    *right_int = INT_MAX;
    if (left != NULL) left_int = (int *) left;
    if (right != NULL) right_int = (int *) right;
    if (*left_int > *right_int) result += 1;
    if (*left_int < *right_int) result -= 1;
    free(left_int);
    free(right_int);
    return result;
}

void test_new_heap()
{
    heap_t * heap = heap_new(*fn_int_cmp);
    CU_ASSERT_PTR_NOT_NULL_FATAL(heap);
    CU_ASSERT_PTR_NOT_NULL(heap->vec);
    CU_ASSERT_PTR_NOT_NULL(heap->cmp);
}

CU_pSuite suite_heap_test()
{
    CU_pSuite suite = CU_add_suite("vector_test", NULL, NULL);
    
    CU_add_test(suite, "test_new_heap", test_new_heap);

    return suite;
}
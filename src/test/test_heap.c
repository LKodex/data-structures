#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "heap.h"

CU_pSuite suite_heap_test()
{
    CU_pSuite suite = CU_add_suite("vector_test", NULL, NULL);
    return suite;
}
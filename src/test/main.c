#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "test_vector.c"

int main()
{
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    suite_vector_test();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
}

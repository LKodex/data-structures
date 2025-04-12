#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "vector.h"

void test_new_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    CU_ASSERT_PTR_NOT_NULL(vector);
    CU_ASSERT_EQUAL(vector->capacity, 1);
    CU_ASSERT_EQUAL(vector->size, 0);
    CU_ASSERT_PTR_NOT_NULL(vector->elements);
    vector = vector_destroy(vector);
}

void test_destroy_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    vector = vector_destroy(vector);
    CU_ASSERT_PTR_NULL(vector);
}

void test_destroy_vector_with_zero_capacity_after_destroyed()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    vector = vector_destroy(vector);
    vector = vector_destroy(vector);
    CU_ASSERT_PTR_NULL(vector);
}

void test_add_element_to_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    size_t * element = malloc(sizeof(size_t));
    *element = 0;
    vector_add(vector, element);
    CU_ASSERT_EQUAL(vector->capacity, 1);
    CU_ASSERT_EQUAL(vector->size, 1);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[0], 0);
    vector = vector_destroy(vector);
}

void test_remove_element_from_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    size_t * element = (size_t *) vector_remove(vector, 0);
    CU_ASSERT_PTR_NULL(element);
    CU_ASSERT_EQUAL(vector->capacity, 1);
    CU_ASSERT_EQUAL(vector->size, 0);
    vector = vector_destroy(vector);
}

void test_get_element_from_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    size_t * element = vector_get(vector, 0);
    CU_ASSERT_PTR_NULL(element);
    vector = vector_destroy(vector);
}

void test_set_element_from_vector_with_zero_capacity()
{
    const size_t capacity = 0;
    vector_t * vector = vector_new(capacity);
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 0;
    size_t * old_element = vector_set(vector, 0, element);
    CU_ASSERT_PTR_NULL(old_element);
    CU_ASSERT_EQUAL(vector->capacity, 1);
    CU_ASSERT_EQUAL(vector->size, 1);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[0], 0);
    vector = vector_destroy(vector);
}

void test_new_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 0);
    CU_ASSERT_PTR_NOT_NULL(vector->elements);
    vector = vector_destroy(vector);
}

void test_destroy_empty_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    vector = vector_destroy(vector);
    CU_ASSERT_PTR_NULL(vector);
}

void test_destroy_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    vector = vector_destroy(vector);
    CU_ASSERT_PTR_NULL(vector);
}

void test_add_one_element_to_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    size_t * element = malloc(sizeof(size_t));
    *element = 0;
    vector_add(vector, element);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 1);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[0], 0);
    vector = vector_destroy(vector);
}

void test_add_ten_elements_to_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
        CU_ASSERT_EQUAL(vector->capacity, 10);
        CU_ASSERT_EQUAL(vector->size, i + 1);
        CU_ASSERT_EQUAL(* (size_t *) vector->elements[i], *element);
    }
    vector = vector_destroy(vector);
}

void test_remove_element_from_empty_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    size_t * element = (size_t *) vector_remove(vector, 0);
    CU_ASSERT_PTR_NULL(element);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 0);
    vector = vector_destroy(vector);
}

void test_remove_every_element_from_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    for (size_t i = capacity; i > 0; i--)
    {
        size_t * element = (size_t *) vector_remove(vector, i - 1);
        CU_ASSERT_PTR_NOT_NULL(element);
        CU_ASSERT_EQUAL(vector->capacity, 10);
        CU_ASSERT_EQUAL(vector->size, i - 1);
        free(element);
    }
    vector = vector_destroy(vector);
}

void test_get_first_element_from_empty_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    size_t * element = vector_get(vector, 0);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 0);
    CU_ASSERT_PTR_NULL(element);
    vector = vector_destroy(vector);
}

void test_get_first_element_from_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = vector_get(vector, 0);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 10);
    CU_ASSERT_PTR_NOT_NULL(element);
    CU_ASSERT_EQUAL(*element, 0);
    vector = vector_destroy(vector);
}

void test_get_last_element_from_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = vector_get(vector, 9);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 10);
    CU_ASSERT_PTR_NOT_NULL(element);
    CU_ASSERT_EQUAL(*element, 9);
    vector = vector_destroy(vector);
}

void test_get_one_element_after_last_from_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = vector_get(vector, 10);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 10);
    CU_ASSERT_PTR_NULL(element);
    vector = vector_destroy(vector);
}

void test_set_first_element_of_empty_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 0;
    size_t * old_element = vector_set(vector, 0, element);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 1);
    CU_ASSERT_PTR_NULL(old_element);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[0], 0);
    vector = vector_destroy(vector);
}

void test_set_first_element_of_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 10;
    size_t * old_element = vector_set(vector, 0, element);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 10);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[0], 10);
    CU_ASSERT_PTR_NOT_NULL(old_element);
    CU_ASSERT_EQUAL(*old_element, 0);
    free(old_element);
    vector = vector_destroy(vector);
}

void test_set_last_element_of_full_vector_with_ten_capacity()
{
    const size_t capacity = 10;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 19;
    size_t * old_element = vector_set(vector, 9, element);
    CU_ASSERT_EQUAL(vector->capacity, 10);
    CU_ASSERT_EQUAL(vector->size, 10);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[9], 19);
    CU_ASSERT_PTR_NOT_NULL(old_element);
    CU_ASSERT_EQUAL(*old_element, 9);
    free(old_element);
    vector = vector_destroy(vector);
}

void test_add_one_element_in_full_vector_with_eight_capacity()
{
    const size_t capacity = 8;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 8;
    vector_add(vector, element);
    CU_ASSERT_EQUAL(vector->capacity, 16);
    CU_ASSERT_EQUAL(vector->size, 9);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[8], 8);
    vector = vector_destroy(vector);
}

void test_set_one_element_after_last_of_full_vector_with_eight_capacity()
{
    const size_t capacity = 8;
    vector_t * vector = vector_new(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    size_t * element = (size_t *) malloc(sizeof(size_t));
    *element = 8;
    size_t * old_element = vector_set(vector, 8, element);
    CU_ASSERT_EQUAL(vector->capacity, 16);
    CU_ASSERT_EQUAL(vector->size, 9);
    CU_ASSERT_EQUAL(* (size_t *) vector->elements[8], 8);
    CU_ASSERT_PTR_NULL(old_element);
    vector = vector_destroy(vector);
}

void test_add_elements_up_to_sixteen_size_with_vector_with_eight_capacity()
{
    const size_t initial_capacity = 8;
    vector_t * vector = vector_new(initial_capacity);
    for (size_t i = 0; i < initial_capacity; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    CU_ASSERT_EQUAL(vector->capacity, initial_capacity);
    CU_ASSERT_EQUAL(vector->size, initial_capacity);
    CU_ASSERT_PTR_NOT_NULL(vector->elements);
    for (size_t i = initial_capacity; i < initial_capacity * 2; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    CU_ASSERT_EQUAL(vector->capacity, 16);
    CU_ASSERT_EQUAL(vector->size, 16);
    CU_ASSERT_PTR_NOT_NULL(vector->elements);
    for (size_t i = 0; i < 16; i++)
    {
        CU_ASSERT_EQUAL(* (size_t *) vector->elements[i], i);
    }
    vector = vector_destroy(vector);
}

void test_remove_all_elements_by_first_index_of_vector_with_eight_capacity_grown_to_sixteen()
{
    const size_t initial_capacity = 8;
    vector_t * vector = vector_new(initial_capacity);
    for (size_t i = 0; i < initial_capacity * 2; i++)
    {
        size_t * element = malloc(sizeof(size_t));
        *element = i;
        vector_add(vector, element);
    }
    for (size_t i = 0; i < 16; i++)
    {
        size_t * element = (size_t *) vector_remove(vector, 0);
        CU_ASSERT_PTR_NOT_NULL(element);
        CU_ASSERT_EQUAL(vector->capacity, 16);
        CU_ASSERT_EQUAL(vector->size, 16 - 1 - i);
        free(element);
    }
    vector = vector_destroy(vector);
}

CU_pSuite suite_vector_test()
{
    CU_pSuite suite = CU_add_suite("vector_test", NULL, NULL);
    // zero capacity vector
    CU_add_test(suite, "test_new_vector_with_zero_capacity", test_new_vector_with_zero_capacity);
    CU_add_test(suite, "test_destroy_vector_with_zero_capacity", test_destroy_vector_with_zero_capacity);
    CU_add_test(suite, "test_destroy_vector_with_zero_capacity_after_destroyed", test_destroy_vector_with_zero_capacity_after_destroyed);
    CU_add_test(suite, "test_add_element_to_vector_with_zero_capacity", test_add_element_to_vector_with_zero_capacity);
    CU_add_test(suite, "test_remove_element_from_vector_with_zero_capacity", test_remove_element_from_vector_with_zero_capacity);
    CU_add_test(suite, "test_get_element_from_vector_with_zero_capacity", test_get_element_from_vector_with_zero_capacity);
    CU_add_test(suite, "test_set_element_from_vector_with_zero_capacity", test_set_element_from_vector_with_zero_capacity);
    // ten capacity vector
    CU_add_test(suite, "test_new_vector_with_ten_capacity", test_new_vector_with_ten_capacity);
    CU_add_test(suite, "test_destroy_empty_vector_with_ten_capacity", test_destroy_empty_vector_with_ten_capacity);
    CU_add_test(suite, "test_destroy_full_vector_with_ten_capacity", test_destroy_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_add_one_element_to_vector_with_ten_capacity", test_add_one_element_to_vector_with_ten_capacity);
    CU_add_test(suite, "test_add_ten_elements_to_vector_with_ten_capacity", test_add_ten_elements_to_vector_with_ten_capacity);
    CU_add_test(suite, "test_remove_element_from_empty_vector_with_ten_capacity", test_remove_element_from_empty_vector_with_ten_capacity);
    CU_add_test(suite, "test_remove_every_element_from_full_vector_with_ten_capacity", test_remove_every_element_from_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_get_first_element_from_empty_vector_with_ten_capacity", test_get_first_element_from_empty_vector_with_ten_capacity);
    CU_add_test(suite, "test_get_first_element_from_full_vector_with_ten_capacity", test_get_first_element_from_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_get_last_element_from_full_vector_with_ten_capacity", test_get_last_element_from_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_get_one_element_after_last_from_full_vector_with_ten_capacity", test_get_one_element_after_last_from_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_set_first_element_of_empty_vector_with_ten_capacity", test_set_first_element_of_empty_vector_with_ten_capacity);
    CU_add_test(suite, "test_set_first_element_of_full_vector_with_ten_capacity", test_set_first_element_of_full_vector_with_ten_capacity);
    CU_add_test(suite, "test_set_last_element_of_full_vector_with_ten_capacity", test_set_last_element_of_full_vector_with_ten_capacity);
    // growing vector from eight to sixteen
    CU_add_test(suite, "test_set_one_element_after_last_of_full_vector_with_eight_capacity", test_set_one_element_after_last_of_full_vector_with_eight_capacity);
    CU_add_test(suite, "test_add_elements_up_to_sixteen_size_with_vector_with_eight_capacity", test_add_elements_up_to_sixteen_size_with_vector_with_eight_capacity);
    CU_add_test(suite, "test_remove_all_elements_by_first_index_of_vector_with_eight_capacity_grown_to_sixteen", test_remove_all_elements_by_first_index_of_vector_with_eight_capacity_grown_to_sixteen);
    return suite;
}

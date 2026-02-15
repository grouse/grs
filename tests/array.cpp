#include "core/array.h"
#include "core/test.h"

TEST_PROC(array__indexing)
{
    i32 data[5] = { 1, 2, 3, 4, 5 };
    Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data)-1 };

    ASSERT(arr.count == 4);
    ASSERT(arr[0] == 1);
    EXPECT_FAIL(arr[4] = 6);

    ASSERT(array_tail(arr) == &arr[3]);
    ASSERT(array_tail(arr) == &data[3]);
}

TEST_PROC(array__iterator)
{
    i32 data[5] = { 1, 2, 3, 4, 5 };
    Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data) };

    {
        i32 index = 0;
        for (auto it : arr) {
            ASSERT(it == arr[index]);
            index++;
        }
    }

    {
        i32 index = 0;
        for (auto it : iterator(arr)) {
            ASSERT(it == arr[index]);
            ASSERT(it.index == index);
            index++;
        }
    }

    {
        i32 index = 0;
        for (auto it : reverse(arr)) {
            ASSERT(it == arr[arr.count-index-1]);
            ASSERT(it.index == arr.count-index-1);
            index++;
        }
    }

    {
        i32 *ptrs[5] = { &data[0], &data[1], &data[2], &data[3], &data[4] };
        Array<i32*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : ptrs_arr) {
            ASSERT(it == ptrs[index]);
            ASSERT(it == &data[index]);
            index++;
        }

    }

    {
        struct D { i32 x; };
        D sdata[5] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
        D *ptrs[5] = { &sdata[0], &sdata[1], &sdata[2], &sdata[3], &sdata[4] };
        Array<D*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : ptrs_arr) {
            ASSERT(it == ptrs[index]);
            ASSERT(it == &sdata[index]);
            ASSERT(it->x == sdata[index].x);
            index++;
        }
    }

    {
        struct D { i32 x; };
        D sdata[5] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
        D *ptrs[5] = { &sdata[0], &sdata[1], &sdata[2], &sdata[3], &sdata[4] };
        Array<D*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : iterator(ptrs_arr)) {
            ASSERT(it.index == index);
            ASSERT(it == ptrs[index]);
            ASSERT(it == &sdata[index]);
            ASSERT(it->x == sdata[index].x);
            index++;
        }

    }
}

TEST_PROC(dynamic_array__copy_replaces_contents)
{
    i32 data[] = { 1, 2, 3 };
    Array<i32> src = { data, ARRAY_COUNT(data) };

    {
        DynamicArray<i32> arr{};
        array_copy(&arr, src);

        ASSERT(arr.count == src.count);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 2);
        ASSERT(arr[2] == 3);
    }

    {
        DynamicArray<i32> arr{};
        array_add(&arr, 4);
        array_add(&arr, 5);
        array_add(&arr, 6);
        ASSERT(arr.count == 3);
        array_copy(&arr, src);

        ASSERT(arr.count == src.count);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 2);
        ASSERT(arr[2] == 3);
    }

}


TEST_PROC(dynamic_array__append)
{
    DynamicArray<i32> arr{};
    ASSERT(arr.alloc.proc == nullptr);

    array_add(&arr, 1);
    ASSERT(arr.alloc == mem_dynamic);
    ASSERT(arr.count == 1);
    ASSERT(arr[0] == 1);

    array_append(&arr, { 2, 3 });
    ASSERT(arr.count == 3);
    ASSERT(arr[1] == 2);
    ASSERT(arr[2] == 3);

    i32 ints[2] = { 4, 5 };
    array_add(&arr, ints, ARRAY_COUNT(ints));
    ASSERT(arr.count == 5);
    ASSERT(arr[3] == 4);
    ASSERT(arr[4] == 5);
}

TEST_PROC(dynamic_array__insert)
{
    DynamicArray<i32> arr{};
    ASSERT(arr.alloc.proc == nullptr);

    EXPECT_FAIL(array_insert(&arr, 1, 1));
    ASSERT(arr.count == 0);

    array_insert(&arr, 0, 1);
    ASSERT(arr.count == 1);
    ASSERT(arr[0] == 1);

    array_insert(&arr, 0, 2);
    ASSERT(arr.count == 2);
    ASSERT(arr[0] == 2);
    ASSERT(arr[1] == 1);

    i32 ints[2] = { 3, 4 };
    array_insert(&arr, 1, ints, ARRAY_COUNT(ints));
    ASSERT(arr.count == 4);
    ASSERT(arr[0] == 2);
    ASSERT(arr[1] == 3);
    ASSERT(arr[2] == 4);
    ASSERT(arr[3] == 1);
}

TEST_PROC(dynamic_array__set)
{
    {
        DynamicArray<i32> arr{};
        array_set_grow(&arr, 0, 1);
        ASSERT(arr.count == 1);
        ASSERT(arr.capacity >= 1);
        ASSERT(arr[0] == 1);

        array_set_grow(&arr, 3, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr.capacity >= 4);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 0);
        ASSERT(arr[2] == 0);
        ASSERT(arr[3] == 2);
    }
}

TEST_PROC(dynamic_array__reserve)
{
    DynamicArray<i32> arr{};
    ASSERT(arr.count == 0);
    ASSERT(arr.capacity == 0);
    ASSERT(arr.capacity >= arr.count);

    array_reserve(&arr, 4);
    ASSERT(arr.count == 0);
    ASSERT(arr.capacity >= 4);
    ASSERT(arr.capacity >= arr.count);

    array_reserve(&arr, 2);
    ASSERT(arr.count == 0);
    ASSERT(arr.capacity > 2);
    ASSERT(arr.capacity >= arr.count);

    array_reserve(&arr, 16);
    ASSERT(arr.count == 0);
    ASSERT(arr.capacity >= 16);
    ASSERT(arr.capacity >= arr.count);

    array_add(&arr, 1);
    array_add(&arr, 2);
    ASSERT(arr.count == 2);
    ASSERT(arr.capacity >= arr.count);

    array_reserve(&arr, 32);
    ASSERT(arr.count == 2);
    ASSERT(arr.capacity >= 32);
    ASSERT(arr.capacity >= arr.count);
}

TEST_PROC(dynamic_array__resize)
{
    DynamicArray<i32> arr{};
    ASSERT(arr.count == 0);
    ASSERT(arr.capacity == 0);
    ASSERT(arr.capacity >= arr.count);

    array_resize(&arr, 4);
    ASSERT(arr.count == 4);
    ASSERT(arr.capacity >= 4);
    ASSERT(arr.capacity >= arr.count);

    array_resize(&arr, 2);
    ASSERT(arr.count == 2);
    ASSERT(arr.capacity > 2);
    ASSERT(arr.capacity >= arr.count);

    array_resize(&arr, 16);
    ASSERT(arr.count == 16);
    ASSERT(arr.capacity >= 16);
    ASSERT(arr.capacity >= arr.count);

    array_add(&arr, 1);
    array_add(&arr, 2);
    ASSERT(arr.count == 18);
    ASSERT(arr.capacity >= arr.count);

    array_resize(&arr, 32);
    ASSERT(arr.count == 32);
    ASSERT(arr.capacity >= 32);
    ASSERT(arr.capacity >= arr.count);
}

TEST_PROC(fixed_array__basic_construction)
{
    // Test default construction
    FixedArray<int, 5> arr1;
    ASSERT(arr1.count == 0);
    ASSERT(arr1.data == (int*)arr1.storage);

    // Test initialization list construction
    FixedArray<int, 5> arr2 = {1, 2, 3};
    ASSERT(arr2.count == 3);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr2[0] == 1);
    ASSERT(arr2[1] == 2);
    ASSERT(arr2[2] == 3);

    // Test construction from Array
    int raw_data[] = {4, 5, 6, 7};
    Array<int> source{raw_data, 4};
    FixedArray<int, 5> arr3(source);
    ASSERT(arr3.count == 4);
    ASSERT(arr3.data == (int*)arr3.storage);
    for (int i = 0; i < 4; i++) {
        ASSERT(arr3[i] == raw_data[i]);
    }
}

TEST_PROC(fixed_array__copy_operations)
{
    {
        FixedArray<int, 5> original = {1, 2, 3};
        ASSERT(original.data == (int*)original.storage);

        FixedArray<int, 5> copied(original);
        ASSERT(copied.data == (int*)copied.storage);
        ASSERT(copied.count == 3);

        for (int i = 0; i < 3; i++) {
            ASSERT(copied[i] == original[i]);
        }

        copied[0] = 10;
        ASSERT(original[0] == 1);
        ASSERT(copied[0] == 10);
    }


    {
        FixedArray<int, 5> original = {1, 2, 3};
        ASSERT(original.data == (int*)original.storage);

        FixedArray<int, 5> assigned;
        assigned = original;
        ASSERT(assigned.data == (int*)assigned.storage);
        ASSERT(assigned.count == 3);

        for (int i = 0; i < 3; i++) {
            ASSERT(assigned[i] == original[i]);
        }

        assigned[0] = 10;
        ASSERT(original[0] == 1);
        ASSERT(assigned[0] == 10);
    }
}

TEST_PROC(fixed_array__copy_invokes_copy_constructor_for_each_element)
{
    {
        TestType::reset_counters();

        FixedArray<TestType, 5> original;
        array_add(&original, TestType(1));
        array_add(&original, TestType(2));
        array_add(&original, TestType(3));

        ASSERT(TestType::copy_constructor_calls == 3);
        ASSERT(TestType::move_constructor_calls == 0);
        ASSERT(TestType::copy_assignment_calls == 0);
        ASSERT(TestType::move_assignment_calls == 0);

        FixedArray<TestType, 5> copied(original);

        ASSERT(copied.count == 3);
        ASSERT(copied.data == (TestType*)copied.storage);
        ASSERT(copied.data != original.data);

        ASSERT(TestType::copy_constructor_calls == 6);
        ASSERT(TestType::move_constructor_calls == 0);
        ASSERT(TestType::copy_assignment_calls == 0);
        ASSERT(TestType::move_assignment_calls == 0);

        copied[0].value = 10;
        ASSERT(original[0].value == 1);
        ASSERT(copied[0].value == 10);
    }

    {
        TestType::reset_counters();

        FixedArray<TestType, 5> empty;
        FixedArray<TestType, 5> copied(empty);

        ASSERT(copied.count == 0);
        ASSERT(copied.data == (TestType*)copied.storage);
        ASSERT(TestType::constructor_calls == 0);
        ASSERT(TestType::destructor_calls == 0);
        ASSERT(TestType::copy_constructor_calls == 0);
        ASSERT(TestType::move_constructor_calls == 0);
        ASSERT(TestType::copy_assignment_calls == 0);
        ASSERT(TestType::move_assignment_calls == 0);
    }

    {
        TestType::reset_counters();

        FixedArray<TestType, 5> original;
        array_add(&original, TestType(1));
        array_add(&original, TestType(2));
        array_add(&original, TestType(3));

        FixedArray<TestType, 5> assigned;
        array_add(&assigned, TestType(4));

        assigned = original;

        ASSERT(assigned.count == 3);
        ASSERT(assigned.data == (TestType*)assigned.storage);


        assigned[0].value = 10;
        ASSERT(original[0].value == 1);
        ASSERT(assigned[0].value == 10);
    }
}

TEST_PROC(fixed_array__self_assign_avoids_copies)
{
    TestType::reset_counters();

    FixedArray<TestType, 5> original;
    array_add(&original, TestType(1));
    array_add(&original, TestType(2));
    array_add(&original, TestType(3));

    i32 init_copy_constructor_calls = TestType::copy_constructor_calls;
    i32 init_move_constructor_calls = TestType::move_constructor_calls;
    i32 init_copy_assignment_calls = TestType::copy_assignment_calls;
    i32 init_move_assignment_calls = TestType::move_assignment_calls;

    auto *other = &original;
    original = *other;

    ASSERT(other->data == original.data);
    ASSERT(other->count == original.count);

    ASSERT(TestType::copy_constructor_calls == init_copy_constructor_calls);
    ASSERT(TestType::move_constructor_calls == init_move_constructor_calls);
    ASSERT(TestType::copy_assignment_calls == init_copy_assignment_calls);
    ASSERT(TestType::move_assignment_calls == init_move_assignment_calls);
}


TEST_PROC(fixed_array__size_limits)
{
    // Test truncation when source is larger than capacity
    int raw_data[] = {1, 2, 3, 4, 5, 6};
    Array<int> large_source{raw_data, 6};
    FixedArray<int, 4> truncated(large_source);
    ASSERT(truncated.count == 4);
    for (int i = 0; i < 4; i++) {
        ASSERT(truncated[i] == raw_data[i]);
    }

    // Test initialization list truncation
    FixedArray<int, 3> truncated_init = {1, 2, 3, 4, 5};
    ASSERT(truncated_init.count == 3);
    for (int i = 0; i < 3; i++) {
        ASSERT(truncated_init[i] == i + 1);
    }
}

TEST_PROC(fixed_array__data_pointer_integrity)
{
    FixedArray<int, 5> arr1 = {1, 2, 3};
    const int* original_data = arr1.data;

    FixedArray<int, 5> arr2(arr1);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr1.data == original_data);

    FixedArray<int, 5> arr3;
    arr3 = arr1;
    ASSERT(arr3.data == (int*)arr3.storage);
    ASSERT(arr1.data == original_data);

    FixedArray<int, 5> arr4;
    arr4 = arr3 = arr2 = arr1;
    ASSERT(arr4.data == (int*)arr4.storage);
    ASSERT(arr3.data == (int*)arr3.storage);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr1.data == original_data);
}

TEST_PROC(array__pop)
{
    {
        int data[] = {1, 2, 3, 4};
        Array<int> arr{data, 4};

        ASSERT(array_pop(&arr) == 4);
        ASSERT(arr.count == 3);
        ASSERT(array_pop(&arr) == 3);
        ASSERT(arr.count == 2);
    }
}

TEST_PROC(array__tail)
{
    // Test basic tail access
    {
        int data[] = {1, 2, 3};
        Array<int> arr{data, 3};

        ASSERT(*array_tail(arr) == 3);
        arr.count--;
        ASSERT(*array_tail(arr) == 2);
    }

    // Test empty array
    {
        Array<int> empty{};
        ASSERT(array_tail(empty) == nullptr);
    }
}

TEST_PROC(array__create)
{
    // Test creation with default allocator
    {
        Array<int> arr = array_create<int>(5, mem_dynamic);
        ASSERT(arr.count == 5);
        ASSERT(arr.data != nullptr);

        // Test data can be written to
        for (int i = 0; i < 5; i++) {
            arr.data[i] = i;
            ASSERT(arr[i] == i);
        }
    }
}

TEST_PROC(array__remove)
{
    // Test remove (ordered)
    {
        int data[] = {1, 2, 3, 4, 5};
        Array<int> arr{data, 5};

        array_remove(&arr, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 2);
        ASSERT(arr[2] == 4);
        ASSERT(arr[3] == 5);

        array_remove(&arr, 0);
        ASSERT(arr.count == 3);
        ASSERT(arr[0] == 2);
        ASSERT(arr[1] == 4);
        ASSERT(arr[2] == 5);

        EXPECT_FAIL(array_remove(&arr, 3)); // Out of bounds
    }

    // Test remove_unsorted (unordered)
    {
        int data[] = {1, 2, 3, 4, 5};
        Array<int> arr{data, 5};

        array_remove_unsorted(&arr, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr[2] == 5);

        array_remove_unsorted(&arr, 0);
        ASSERT(arr.count == 3);

        EXPECT_FAIL(array_remove_unsorted(&arr, 3)); // Out of bounds
    }
}

TEST_PROC(array__slice)
{
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr{data, 5};

    // Test full slice
    {
        Array<int> sliced = slice(arr, 0, 5);
        ASSERT(sliced.count == 5);
        ASSERT(sliced.data == arr.data);
    }

    // Test partial slice
    {
        Array<int> sliced = slice(arr, 1, 4);
        ASSERT(sliced.count == 3);
        ASSERT(sliced.data == &arr.data[1]);
        ASSERT(sliced[0] == 2);
        ASSERT(sliced[1] == 3);
        ASSERT(sliced[2] == 4);
    }

    // Test single-argument slice
    {
        Array<int> sliced = slice(arr, 2);
        ASSERT(sliced.count == 3);
        ASSERT(sliced.data == &arr.data[2]);
    }

    // Test edge cases
    {
        Array<int> empty = slice(arr, 5, 5);
        ASSERT(empty.count == 0);

        EXPECT_FAIL(slice(arr, 2, 1)); // Invalid range
        EXPECT_FAIL(slice(arr, -1));   // Invalid start
        EXPECT_FAIL(slice(arr, 0, 6)); // Invalid end
    }
}

TEST_PROC(array__find)
{
    int data[] = {1, 2, 3, 2, 1};
    Array<int> arr{data, 5};

    // Test array_find_index
    {
        ASSERT(array_find_index(arr, 1) == 0); // First occurrence
        ASSERT(array_find_index(arr, 2) == 1);
        ASSERT(array_find_index(arr, 4) == -1); // Not found
    }

    // Test array_find
    {
        ASSERT(*array_find(arr, 1) == 1);
        ASSERT(array_find(arr, 1) == &arr[0]);
        ASSERT(array_find(arr, 4) == nullptr);
    }

    // Test array_contains
    {
        ASSERT(array_contains(arr, 1));
        ASSERT(array_contains(arr, 3));
        ASSERT(!array_contains(arr, 4));
    }

    {
        i32 data[5] = { 1, 2, 3, 4, 5 };
        Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data) };

        ASSERT(*array_find(arr, 1) == 1);
        ASSERT(*array_find(arr, 5) == 5);
        ASSERT(array_find(arr, 6) == nullptr);

        ASSERT(array_find(arr, 1) == &data[0]);
        ASSERT(array_find(arr, 1) == &arr[0]);
        ASSERT(array_find(arr, 5) == &data[4]);
        ASSERT(array_find(arr, 5) == &arr[4]);

        ASSERT(array_find_index(arr, 1) == 0);
        ASSERT(array_find_index(arr, 5) == 4);
        ASSERT(array_find_index(arr, 6) == -1);
    }
}

TEST_PROC(array__swap)
{
    // Test single array swap
    {
        int data[] = {1, 2, 3};
        Array<int> arr{data, 3};

        array_swap(arr, 0, 2);
        ASSERT(arr[0] == 3);
        ASSERT(arr[2] == 1);
    }

    // Test multi-array swap
    {
        int data1[] = {1, 2, 3};
        char data2[] = {'a', 'b', 'c'};
        Array<int> arr1{data1, 3};
        Array<char> arr2{data2, 3};

        array_swap(arr1, 0, 2, arr2);
        ASSERT(arr1[0] == 3 && arr1[2] == 1);
        ASSERT(arr2[0] == 'c' && arr2[2] == 'a');
    }
}

TEST_PROC(array__sort)
{
    // Test exchange sort
    {
        int data[] = {5, 3, 1, 4, 2};
        Array<int> arr{data, 5};

        exchange_sort(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] <= arr[i+1]);
        }
    }

    // Test quick sort ascending
    {
        int data[] = {5, 3, 1, 4, 2};
        Array<int> arr{data, 5};

        quick_sort_asc(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] <= arr[i+1]);
        }
    }

    // Test quick sort descending
    {
        int data[] = {1, 3, 5, 4, 2};
        Array<int> arr{data, 5};

        quick_sort_desc(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] >= arr[i+1]);
        }
    }

    // Test sorting with parallel arrays
    {
        int keys[] = {5, 3, 1, 4, 2};
        char values[] = {'a', 'b', 'c', 'd', 'e'};
        Array<int> key_arr{keys, 5};
        Array<char> value_arr{values, 5};

        quick_sort_asc(key_arr, value_arr);
        ASSERT(key_arr[0] == 1 && value_arr[0] == 'c');
        ASSERT(key_arr[4] == 5 && value_arr[4] == 'a');
    }
}

#include "core/map.h"
#include "core/test.h"

TEST_PROC(dynamic_map__set_invokes_copy_constructor_for_key_and_value)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });

    ASSERT(TestType::copy_constructor_calls == 6);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 0);
    ASSERT(TestType::move_assignment_calls == 0);
}

TEST_PROC(dynamic_map__growing_map_invokes_copy_constructors)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });

    void *old = map.slots;
    map_grow(&map, map.capacity+2);
    ASSERT(map.slots != old);

    ASSERT(TestType::copy_constructor_calls == 12);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 0);
    ASSERT(TestType::move_assignment_calls == 0);
}

TEST_PROC(dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });
    map_set(&map, { 2 }, { 1 });

    ASSERT(TestType::copy_constructor_calls == 6);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 1);
    ASSERT(TestType::move_assignment_calls == 0);
}

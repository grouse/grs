#define TEST_H_IMPL
#include "test.h"

#include "core.cpp"
#include "maths.cpp"
#include "string.cpp"
#include "memory.cpp"
#include "thread.cpp"
#include "map.h"

#include "generated/tests/tests.h"
#include "MurmurHash/MurmurHash3.cpp"


int main()
{
    mem_sys = malloc_allocator();
    mem_dynamic = malloc_allocator();
    return run_tests(TESTS_tests, sizeof(TESTS_tests) / sizeof(TESTS_tests[0]));
}

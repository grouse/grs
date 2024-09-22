#define TEST_H_IMPL
#include "test.h"

#include "maths.cpp"
#include "core.cpp"
#include "string.cpp"
#include "memory.cpp"
#include "thread.cpp"

#include "generated/tests/tests.h"


int main()
{
    mem_sys = malloc_allocator();
    mem_dynamic = malloc_allocator();
    return run_tests(TESTS_tests, sizeof(TESTS_tests) / sizeof(TESTS_tests[0]));
}

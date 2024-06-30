#include "maths.cpp"
#include "core.cpp"
#include "string.cpp"
#include "memory.cpp"
#include "thread.cpp"

#include "gen/tests/tests.h"

#define TEST_H_IMPL
#include "test.h"

int main()
{
    return run_tests(TESTS_tests, sizeof(TESTS_tests) / sizeof(TESTS_tests[0]));
}

#define TEST_H_IMPL
#include "core/test.h"

#include "core/array.h"
#include "core/string.h"

#include "generated/tests/maths.h"
#include "generated/tests/array.h"
#include "generated/tests/map.h"
#include "generated/tests/memory.h"

int main(Array<String> args)
{
    extern Allocator mem_sys;
    mem_sys = malloc_allocator();
    mem_dynamic = malloc_allocator();

    TestStats stats{};
    RUN_TESTS(MATHS__tests,  &stats);
    RUN_TESTS(MAP__tests,    &stats);
    RUN_TESTS(ARRAY__tests,  &stats);
    RUN_TESTS(MEMORY__tests, &stats);

    test_print_summary(&stats);
    return stats.failed;
}

#define TEST_H_IMPL
#include "core/test.h"

#include "core/array.h"
#include "core/string.h"

#include "generated/maths.h"
#include "generated/array.h"
#include "generated/map.h"
#include "generated/memory.h"
#include "generated/string.h"

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
    RUN_TESTS(STRING__tests, &stats);

    test_print_summary(&stats);
    return stats.failed;
}

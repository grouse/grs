#define TEST_H_IMPL
#include "core/test.h"

#include "core/array.h"
#include "core/string.h"

#include "generated/tests/maths.h"
#include "generated/tests/array.h"
#include "generated/tests/map.h"

int main(Array<String> args)
{
    extern Allocator mem_sys;
    mem_sys = malloc_allocator();
    mem_dynamic = malloc_allocator();

    int error = 0;
    error |= RUN_TESTS(MATHS__tests);
    error |= RUN_TESTS(MAP__tests);
    error |= RUN_TESTS(ARRAY__tests);
    return error;
}

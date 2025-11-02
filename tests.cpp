#define TEST_H_IMPL
#include "test.h"

#include "array.h"
#include "map.h"
#include "maths.h"

#include "generated/tests/array.h"
#include "generated/tests/map.h"
#include "generated/tests/maths.h"

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

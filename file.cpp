#include "file.h"

#if defined(_WIN32)
#include "win32_file.cpp"
#elif defined(__linux__)
#include "linux_file.cpp"
#else
#error "unsupported platform"
#endif

String uri_from_path(String path, Allocator mem) EXPORT
{
    SArena scratch = tl_scratch_arena(mem);
    path = absolute_path(path, scratch);
    path = to_lower(path, scratch);
    for (char &c : path) if (c == '\\') c = '/';

    return stringf(mem, "file:///%.*s", STRFMT(path));
}

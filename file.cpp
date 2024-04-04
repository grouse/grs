#include "file.h"

#if defined(_WIN32)
#include "win32_file.cpp"
#elif defined(__linux__)
#include "linux_file.cpp"
#else
#error "unsupported platform"
#endif

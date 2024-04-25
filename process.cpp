#include "process.h"

#if defined(_WIN32)
#include "win32_process.cpp"
#elif defined(__linux__)
#include "linux_process.cpp"
#else
#error "unsupported platform"
#endif

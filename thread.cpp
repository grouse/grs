#include "thread.h"

#if defined(_WIN32)
#include "win32_thread.cpp"
#elif defined(__linux__)
#include "linux_thread.cpp"
#else
#error "unsupported platform"
#endif

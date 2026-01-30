#define JL_CORE_IMPL

#include "core.h"
#include "string.h"

#if defined(_WIN32)
#include "win32_core.cpp"
#elif defined(__linux__)
#include "linux_core.cpp"
#else
#error "unsupported platform"
#endif

#include <string.h>

i32 jl_next_type_id = 0;

bool default_assert_handler(const char *file, int line, const char *sz_cond)
{
    logf(file, line, LOG_TYPE_ERROR, "assertion failed: %s", sz_cond);
    return true;
}
jl_assert_handler_proc jl_assert_handler = default_assert_handler;

bool default_panic_handler(
    const char *file, int line,
    const char *sz_cond,
    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (!sz_cond) logv(file, line, LOG_TYPE_PANIC, fmt, args);
    else {
        char buffer[2048] = "";
        if (fmt) {
            strncat(buffer, fmt, sizeof buffer-1);
            strncat(buffer, ": ", sizeof buffer-1);
        }
        strncat(buffer, sz_cond, sizeof buffer-1);
        logv(file, line, LOG_TYPE_PANIC, buffer, args);
    }
    va_end(args);

    return true;
}
jl_panic_handler_proc jl_panic_handler = default_panic_handler;


bool add_log_sink(sink_proc_t sink)
{
    if (log_sinks.count == log_sinks.capacity()) {
        LOG_ERROR("maximum number of log sinks exceeded");
        return false;
    }

    array_add(&log_sinks, sink);
    return true;
}

void log(const char *file, u32 line, LogType type, const char *msg)
{
    for (auto sink : log_sinks) sink(file, line, type, msg);
}


void logf(const char *path, u32 line, LogType type, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logv(path, line, type, fmt, args);
    va_end(args);
}

void logv(const char *path, u32 line, LogType type, const char *fmt, va_list args)
{
    char buffer[2048];
    i32 length = vsnprintf(buffer, sizeof buffer-1, fmt, args);
    buffer[MIN(length, (i32)sizeof buffer-1)] = '\0';
    log(path, line, type, buffer);
}

void stdio_sink(const char *path, u32 line, LogType type, const char *msg)
{
    FILE *out = stdout;
    switch (type) {
    case LOG_TYPE_INFO:
        break;
    case LOG_TYPE_PANIC:
    case LOG_TYPE_ERROR:
        out = stderr;
    }

    String filename = filename_of_sz(path);
    const char *type_s = sz_from_enum(type);

    char color = 49;
    switch (type) {
    case LOG_TYPE_ERROR: color = 31; break;
    case LOG_TYPE_INFO:  color = 36; break;
    case LOG_TYPE_PANIC: color = 91; break;
    }
    fprintf(out, "%.*s:%d \033[%dm%s\033[m: %s\n", STRFMT(filename), line, color, type_s, msg);
}

void sleep(i32 milliseconds) EXPORT;
i32 current_process_id() EXPORT;

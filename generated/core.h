#ifndef CORE_GENERATED_H
#define CORE_GENERATED_H

extern void log(const char *file, u32 line, LogType type, const char *msg);
extern void logf(const char *file, u32 line, LogType type, const char *fmt, ...);
extern void logv(const char *file, u32 line, LogType type, const char *fmt, int args);
extern bool add_log_sink(sink_proc_t sink);
extern bool init_file_log_sink(const char *app_name, const char *filename);
extern bool debugger_attached();
extern u64 wall_timestamp();
extern f32 wall_duration_s(u64 start, u64 end);
extern f32 app_time_s();
extern bool default_assert_handler(const char *file, int line, const char *sz_cond);
extern bool default_panic_handler(const char *file, int line, const char *sz_cond, const char *fmt, ...);
extern bool add_log_sink(sink_proc_t sink);
extern void file_log_sink(const char *path, u32 line, LogType type, const char *msg);
extern bool init_file_log_sink(const char *sz_app_name, const char *sz_filename);
extern void log(const char *file, u32 line, LogType type, const char *msg);
extern void logf(const char *path, u32 line, LogType type, const char *fmt, ...);
extern void logv(const char *path, u32 line, LogType type, const char *fmt, va_list args);
extern void stdio_sink(const char *path, u32 line, LogType type, const char *msg);
extern void sleep(i32 milliseconds);
extern i32 current_process_id();

#endif // CORE_GENERATED_H

#ifdef CORE_GENERATED_IMPL
#define CORE_INTERNAL
#endif

#if defined(CORE_INTERNAL) && !defined(CORE_INTERNAL_ONCE)
#define CORE_INTERNAL_ONCE

static void close_file_log_sink();

#endif

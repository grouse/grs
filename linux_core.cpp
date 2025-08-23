#include "core.h"
#include "array.h"

#include <ctime>
#include <stdarg.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <X11/cursorfont.h>
#include <X11/Xlib.h>

String jl_exe_path;

extern void stdio_sink(const char *path, u32 line, LogType type, const char *msg);
FixedArray<sink_proc_t, 10> log_sinks{ stdio_sink };


bool debugger_attached()
{
    return false;
}

timespec get_timespec(clockid_t clock)
{
    timespec ts;
    clock_gettime(clock, &ts);
    return ts;
}

u64 wall_timestamp()
{
    timespec ts = get_timespec(CLOCK_MONOTONIC);
	return (u64)ts.tv_sec*1000000000 + (u64)ts.tv_nsec;
}

f32 wall_duration_s(u64 start, u64 end)
{
    return (f32)(end-start)/1000000000;
}

f32 app_time_s()
{
    static timespec start_time = get_timespec(CLOCK_MONOTONIC);
    timespec now = get_timespec(CLOCK_MONOTONIC);
    timespec delta = { now.tv_sec - start_time.tv_sec, now.tv_nsec - start_time.tv_nsec };
    return (f32)delta.tv_sec + (f32)delta.tv_nsec/1000000000;
}

i32 current_process_id() EXPORT
{
    return getpid();
}

int main(int argc, char **argv)
{
    init_default_allocators();

	char *p = last_of(argv[0], '/');
	if (argv[0][0] == '/') {
		jl_exe_path = { argv[0], (i32)(p-argv[0]) };
	} else {
		char buffer[PATH_MAX];
		char *wd = getcwd(buffer, sizeof buffer);
		PANIC_IF(wd == nullptr, "current working dir exceeds PATH_MAX");

		jl_exe_path = join_path(
			{ wd, (i32)strlen(wd) },
			{ argv[0], (i32)(p-argv[0]) },
			mem_dynamic);
	}

	Array<String> args{};
	if (argc > 1) {
		array_create(&args, argc-1, mem_dynamic);
		for (i32 i = 1; i < argc; i++) {
			args[i-1] = String{ argv[i], (i32)strlen(argv[i]) };
		}
	}

    extern int jl_main(Array<String> args);
    return jl_main(args);
}

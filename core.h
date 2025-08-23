#ifndef CORE_H
#define CORE_H

#include <stdarg.h>

#ifndef NULL
#define NULL nullptr
#endif

#if defined(__clang__)

#define PACKED __attribute__((__packed__))

#define EXPORT __attribute__((annotate("export")))
#define EXPORTS namespace PUBLIC __attribute__((annotate("export")))

#define INTERNAL __attribute__((annotate("internal")))
#define INTERNALS namespace __attribute__((annotate("internal")))

#else
#error "unsupported compiler"
#endif

#if defined(__linux__)

#include <linux/limits.h>

#define NOTHROW __attribute__(( __nothrow__ ))
#define CRTIMP

#define DEBUG_BREAK() asm("int $3") // TODO(jesper): remove in non-debug builds
#define BREAK() asm("int $3")


typedef unsigned long size_t;

typedef unsigned long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8 ;

typedef signed long i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8 ;

typedef float f32;
typedef double f64;

#define atomic_exchange(var, value) __sync_lock_test_and_set(var, value)

#elif defined(_WIN32)

#define NOTHROW
#define CRTIMP __declspec(dllimport)

#define DEBUG_BREAK() __debugbreak() // TODO(jesper): remove in non-debug builds
#define BREAK() __debugbreak()

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8 ;

typedef signed long long i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8 ;

typedef float f32;
typedef double f64;


#else
#error "unsupported platform"
#endif

constexpr f64 f64_PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
constexpr f32 f32_PI = (f32)f64_PI;

#define f32_MIN __FLT_MIN__
#define f32_MAX 3.402823466e+38F
#define f32_EPSILON __FLT_EPSILON__
#define f32_INF ((f32)(1e+300*1e+300))

#define i16_MAX (i16)0x7FFF
#define i32_MAX (i32)0x7FFFFFFF

#define u16_MAX (u16)0xFFFF
#define u32_MAX (u32)0xFFFFFFFF
#define u64_MAX (u64)0xFFFFFFFFFFFFFFFF



static_assert(sizeof(u64) == 8, "u64 sizeof mismatch");
static_assert(sizeof(i64) == 8, "i64 sizeof mismatch");
static_assert(sizeof(f64) == 8, "f64 sizeof mismatch");

static_assert(sizeof(u32) == 4, "u32 sizeof mismatch");
static_assert(sizeof(i32) == 4, "i32 sizeof mismatch");
static_assert(sizeof(f32) == 4, "f32 sizeof mismatch");

static_assert(sizeof(u16) == 2, "u16 sizeof mismatch");
static_assert(sizeof(i16) == 2, "i16 sizeof mismatch");

static_assert(sizeof(u8) == 1, "u8 sizeof mismatch");
static_assert(sizeof(i8) == 1, "i8 sizeof mismatch");

#define jl_typeid(T) type_id<T>(#T)
#define defer auto defer_( __LINE__ ) = DeferDummy( ) + [&]( )
#define transmute(T, value) (*(T*)&(value))

#define CAT_(a, b) a ## b
#define CAT(a, b) CAT_(a, b)
#define CAT3(a, b, c) CAT(CAT(a, b), c)
#define CAT4(a, b, c, d) CAT(CAT3(a, b, c), d)

#define VAR(v) CAT(v, __LINE__)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) MAX(MAX(a, b), c)
#define MAX4(a, b, c, d) MAX(MAX(a, b), MAX(c, d))

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MIN3(a, b, c) MIN(MIN(a, b), c)
#define MIN4(a, b, c, d) MIN(MIN(a, b), MIN(c, d))

#define CLAMP(v, min, max) MIN(MAX(v, min), max)


#define SWAP(a, b) do { auto tmp = a; a = b; b = tmp; } while(0)
#define ROUND_TO(value, multiple) ((value) + (multiple) - ((value) % (multiple)))


#define ARRAY_COUNT(arr) (i32)(sizeof(arr) / sizeof ((arr)[0]))


#define ARGS(...) , ##__VA_ARGS__

#define RMOV(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define RFWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)


#define SCOPE_EXPR(expr1, expr2) (expr1); for (i32 CAT(i, __LINE__) = 0; CAT(i, __LINE__) == 0; CAT(i, __LINE__) = ((expr2), 1))
#define SCOPE_CEXPR(expr1, expr2) for (i32 CAT(i, __LINE__) = 0; CAT(i, __LINE__) == 0 && (expr1); CAT(i, __LINE__) = ((expr2), 1))


#define LOGR(...)      logf(__VA_ARGS__)
#define LOG(...)       logf(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logf(__FILE__, __LINE__, LOG_TYPE_ERROR, __VA_ARGS__)
#define LOG_INFO(...)  logf(__FILE__, __LINE__, LOG_TYPE_INFO, __VA_ARGS__)

typedef bool (*assert_handler_t)(const char *src, int line, const char *sz_cond);
typedef bool (*panic_handler_t)(const char *src, int line, const char *sz_cond, const char *msg, ...);

extern assert_handler_t assert_handler;
extern panic_handler_t panic_handler;

#define ASSERT_HANDLER(cond) assert_handler(__FILE__, __LINE__, #cond)
#define PANIC_HANDLER(sz_cond, ...) panic_handler(__FILE__, __LINE__, sz_cond, __VA_ARGS__)

#ifndef ASSERT
#define ASSERT(...) do {\
    if (!(__VA_ARGS__) && ASSERT_HANDLER((__VA_ARGS__))) {\
        DEBUG_BREAK();\
    }\
} while(0)
#endif // ASSERT

#ifndef PANIC
#define PANIC(...) do {\
    if (PANIC_HANDLER(nullptr, __VA_ARGS__)) {\
        DEBUG_BREAK();\
    }\
} while(0)

#endif // PANIC

#ifndef PANIC_IF
#define PANIC_IF(cond, ...) do {\
    if ((cond) && PANIC_HANDLER(#cond, __VA_ARGS__)) {\
        DEBUG_BREAK();\
    }\
} while(0)
#endif // PANIC_IF

#ifndef PANIC_UNREACHABLE
#define PANIC_UNREACHABLE() PANIC("unreachable code reached")
#endif // PANIC_IF

template <typename F>
struct Defer {
    Defer(F f) : f(f) {}
    ~Defer() { f(); }
    F f;
};

template <typename F>
Defer<F> defer_create( F f ) {
    return Defer<F>( f );
}

#define defer__(line) defer_ ## line
#define defer_(line) defer__( line )

struct DeferDummy { };
template<typename F>
Defer<F> operator+ (DeferDummy, F&& f)
{
    return defer_create<F>(RFWD(f));
}


enum LogType {
    LOG_TYPE_PANIC,
    LOG_TYPE_ERROR,
    LOG_TYPE_INFO,
};

inline const char* sz_from_enum(LogType type)
{
    switch (type) {
    case LOG_TYPE_PANIC: return "panic";
    case LOG_TYPE_ERROR: return "error";
    case LOG_TYPE_INFO: return "info";
    }

    return "";
}

typedef void (*sink_proc_t)(const char *src, u32 line, LogType type, const char *msg);
void log(const char *file, u32 line, LogType type, const char *msg);
void logf(const char *file, u32 line, LogType type, const char *fmt, ...);
void logv(const char *file, u32 line, LogType type, const char *fmt, va_list args) ;
bool add_log_sink(sink_proc_t sink);

template<typename T>
i32 type_id(const char *type_name)
{
    extern i32 next_type_id;

    i32 t = next_type_id;
    static i32 id = ++next_type_id;

    if (t != next_type_id) LOG_INFO("registered type [%d] %s", t, type_name);
    return id;
}


bool debugger_attached();

u64 wall_timestamp();
f32 wall_duration_s(u64 start, u64 end = wall_timestamp());

f32 app_time_s();


#include "generated/core.h"

#ifndef JL_CORE_IMPL
#define main jl_main
#endif

#endif // CORE_H

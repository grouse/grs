#ifndef CORE_H
#define CORE_H

#include "string.h"
#include "platform.h"

#include <stdarg.h>

extern "C" CRTIMP void exit(int status) NOTHROW;
extern "C" CRTIMP char* strerror(int errnum) NOTHROW;

#define typeid(T) type_id<T>(#T)
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

void set_clipboard_data(String str);
String read_clipboard_str(Allocator mem);

u64 wall_timestamp();
f32 wall_duration_s(u64 start, u64 end = wall_timestamp());

f32 app_time_s();

// NOTE(jesper): this should be in memory, but I hate C++
struct MemoryBuffer {
    u8 *data;
    i32 size;
    i32 offset;
};

inline void* read_memory(MemoryBuffer *buf, i32 size)
{
    PANIC_IF(buf->offset + size > buf->size, "reading beyond end of buffer");
    void *ptr = (buf->data + buf->offset);
    buf->offset += size;
    return ptr;
}

inline void write_memory(MemoryBuffer *buf, const void *data, i32 size)
{
    PANIC_IF(buf->offset + size > buf->size, "reading beyond end of buffer");
    memcpy(buf->data+buf->offset, data, size);
    buf->offset += size;
}


template<typename T>
inline T read_memory(MemoryBuffer *buf)
{
    PANIC_IF(buf->offset + (i32)sizeof(T) > buf->size, "reading beyond end of buffer");
    T *val = (T*)(buf->data + buf->offset);
    buf->offset += sizeof(T);
    return *val;
}

template<typename T>
inline void write_memory(MemoryBuffer *buf, T val)
{
    PANIC_IF(buf->offset + (i32)sizeof(T) > buf->size, "reading beyond end of buffer");
    memcpy(buf->data+buf->offset, &val, sizeof(T));
    buf->offset += sizeof(T);
}

String read_memory(MemoryBuffer *buf, Allocator mem);

template<>
inline void write_memory(MemoryBuffer *buf, String str)
{
    PANIC_IF(buf->offset + (i32)sizeof(i32) + str.length > buf->size, "reading beyond end of buffer");
    write_memory(buf, str.length);
    write_memory(buf, str.data, str.length);
}

#include "generated/core.h"

#endif // CORE_H

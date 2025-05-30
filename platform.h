#ifndef PLATFORM_H
#define PLATFORM_H

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

#define NOTHROW __attribute__(( __nothrow__ __LEAF))
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

#endif // PLATFORM_H

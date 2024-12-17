#ifndef MATHS_H
#define MATHS_H

#include "platform.h"
#include "core.h"

extern "C" CRTIMP double floor (double x) NOTHROW;
extern "C" CRTIMP float floorf(float x) NOTHROW;
extern "C" CRTIMP float powf( float base, float exponent ) NOTHROW;
extern "C" CRTIMP double ceil (double x) NOTHROW;
extern "C" CRTIMP float ceilf (float x) NOTHROW;
extern "C" double fabs( double arg ) NOTHROW;
extern "C" float fabsf( float arg ) NOTHROW;
extern "C" CRTIMP float cosf( float arg ) NOTHROW;
extern "C" CRTIMP float acosf( float arg ) NOTHROW;
extern "C" CRTIMP float tanf( float arg ) NOTHROW;
extern "C" CRTIMP float sinf( float arg ) NOTHROW;
extern "C" double sin( double arg ) NOTHROW;
extern "C" CRTIMP float sqrtf( float arg ) NOTHROW;
extern "C" double sqrt( double arg ) NOTHROW;
extern "C" CRTIMP float copysignf(float x, float y) NOTHROW;
extern "C" CRTIMP float fmodf( float x, float y ) NOTHROW;

#ifndef M_BOUNDS_CHECK
#define M_BOUNDS_CHECK 0
#endif

#if M_BOUNDS_CHECK
#define M_SUBSCRIPT_OPS(T, values)\
    constexpr T& operator[](i32 i) { ASSERT((i) < ARRAY_COUNT((values))); return values[i]; }\
    constexpr const T& operator[](i32 i) const { ASSERT((i) < ARRAY_COUNT((values))); return values[i]; }
#else
#define M_SUBSCRIPT_OPS(T, values)\
    constexpr T& operator[](i32 i) { return values[i]; }\
    constexpr const T& operator[](i32 i) const { return values[i]; }
#endif


#define M_ADD_OPS(R, T_l, T_r)\
    R operator+(T_l lhs,   T_r rhs);\
    R operator+=(T_l &lhs, T_r rhs)\

#define M_SUB_OPS(R, T_l, T_r)\
    R operator-(T_l lhs,   T_r rhs);\
    R operator-=(T_l &lhs, T_r rhs)

#define M_MUL_OPS(R, T_l, T_r)\
    R operator*(T_l lhs,   T_r rhs);\
    R operator*=(T_l &lhs, T_r rhs)

#define M_DIV_OPS(R, T_l, T_r)\
    R operator/(T_l lhs,   T_r rhs);\
    R operator/=(T_l &lhs, T_r rhs)

#define M_CMP_OPS(T)\
    bool operator==(const T &lhs, const T &rhs);\
    bool operator!=(const T &lhs, const T &rhs)

#define M_UNARY_OPS(T)\
    T operator-(T v);\
    T operator+(T v)


#define M_VECTOR_OPS(T)\
    M_ADD_OPS(T, T, T);\
    M_ADD_OPS(T, T, f32);\
    M_SUB_OPS(T, T, T);\
    M_SUB_OPS(T, T, f32);\
    M_MUL_OPS(T, T, T);\
    M_MUL_OPS(T, T, f32);\
    M_MUL_OPS(T, f32, T);\
    M_DIV_OPS(T, T, T);\
    M_DIV_OPS(T, T, f32);\
    M_CMP_OPS(T);\
    M_UNARY_OPS(T)

#define M_MATRIX_OPS(T, V)\
    M_ADD_OPS(T, T, T);\
    M_SUB_OPS(T, T, T);\
    M_MUL_OPS(T, T, T);\
    M_MUL_OPS(V, T, V);\
    M_DIV_OPS(T, T, T)


struct Vector2 {
    union {
        struct { f32 x, y; };
        struct { f32 w, h; };
        struct { f32 u, v; };
        struct { f32 r, g; };
        f32 data[2];
    };

    M_SUBSCRIPT_OPS(f32, data);
};

struct Vector2i {
    union {
        struct { i32 x, y; };
        struct { i32 w, h; };
        i32 data[2];
    };

    M_SUBSCRIPT_OPS(i32, data);
};
M_VECTOR_OPS(Vector2);

struct Vector3 {
    union {
        struct { f32 x, y, z; };
        struct { Vector2 xy; f32 _z; };
        struct { f32 _x; Vector2 yz; };

        struct { f32 r, g, b; };
        struct { Vector2 rg; f32 _b; };
        struct { f32 _r; Vector2 gb; };

        struct { f32 w, h, d; };
        struct { Vector2 wh; f32 _d; };
        struct { f32 _w; Vector2 hd; };
        f32 data[3];
    };

    M_SUBSCRIPT_OPS(f32, data);
};
M_VECTOR_OPS(Vector3);

struct Vector4 {
    union {
        struct { f32 x, y, z, w; };
        struct { Vector2 xy; Vector2 zw; };
        struct { Vector3 xyz; f32 _w; };
        struct { f32 _x; Vector3 yzw; };

        struct { f32 r, g, b, a; };
        struct { Vector3 rgb; f32 _a; };
        struct { Vector2 rg; Vector2 ba; };
        struct { f32 _r; Vector3 gba; };
        f32 data[4];
    };

    M_SUBSCRIPT_OPS(f32, data);
};
M_VECTOR_OPS(Vector4);

struct Matrix3 {
    union {
        Vector3 columns[3];
        struct { Vector3 col0, col1, col2; };
        struct {
            f32 m00, m10, m20;
            f32 m01, m11, m21;
            f32 m02, m12, m22;
        };
        f32 data[9];
    };

    M_SUBSCRIPT_OPS(Vector3, columns);
};
M_MATRIX_OPS(Matrix3, Vector3);


struct Matrix4 {
    union {
        Vector4 columns[4];
        struct { Vector4 col0, col1, col2, col3; };
        struct {
            f32 m00, m10, m20, m30;
            f32 m01, m11, m21, m31;
            f32 m02, m12, m22, m32;
            f32 m03, m13, m23, m33;
        };
        f32 data[16];
    };

    M_SUBSCRIPT_OPS(Vector4, columns);
    constexpr Vector4 row(i32 i)
    {
        return { columns[0][i], columns[1][i], columns[2][i], columns[3][i] };
    }
};
M_MATRIX_OPS(Matrix4, Vector4);


struct Quaternion {
    union {
        struct { f32 x, y, z, w; };
        struct { Vector3 v; f32 s; };
        struct { Vector3 xyz; f32 _w; };
        Vector4 xyzw;
        f32 data[4];
    };

    M_SUBSCRIPT_OPS(f32, data);
};
M_DIV_OPS(Quaternion, Quaternion, f32);

struct Rect {
    union {
        struct { Vector2 tl, br; };
        Vector2 data[2];
    };

    Rect() = default;
    Rect(Vector2 tl, Vector2 br) : tl(tl), br(br) {}

    constexpr Vector2 pos() const { return tl; }
    constexpr Vector2 pos(Vector2 pos)
    {
        Vector2 size = br-tl;
        tl = pos;
        br = tl+size;
        return tl;
    }

    constexpr Vector2 size() const { return br - tl; }
    constexpr Vector2 size(Vector2 size)
    {
        br = tl+size;
        return size;
    }

    constexpr f32 size_x(f32 x)
    {
        br.x = tl.x + x;
        return x;
    }
    constexpr f32 size_y(f32 y)
    {
        br.y = tl.y + y;
        return y;
    }

    constexpr Vector2& operator[](i32 i) { return data[i]; }
};

bool operator==(const Rect &lhs, const Rect &rhs);

struct Contact {
    Vector2 point;
    Vector2 normal;
    f32 depth;

    operator bool() const { return depth != f32_MAX; }
};

constexpr Contact INVALID_CONTACT = { .depth = f32_MAX };

struct AABB2 {
    Vector2 position;
    Vector2 extents;
};

struct Triangle2 {
    Vector2 p0, p1, p2;
};

struct Line2 {
    Vector2 p0, p1;
};

struct XORShift128 {
    u32 state[4];
};

#include "generated/maths.h"



u32 rand_u32(XORShift128 *series);
f32 rand_f32(XORShift128 *series);
f32 rand_f32(XORShift128 *series, f32 min, f32 max);


u32 bgra_pack(f32 r, f32 g, f32 b, f32 a);
u32 rgb_pack(Vector3 c);
u32 bgr_pack(Vector3 c);
Vector3 bgr_unpack(u32 argb);
Vector4 bgra_unpack(u32 argb);
Vector3 rgb_from_hsl(f32 h, f32 s, f32 l);
Vector3 rgb_mul(Vector3 rgb, f32 v);


bool point_in_aabb(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size, f32 epsilon = 0.0f);

#endif // MATHS_H

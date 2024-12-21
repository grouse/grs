#include "maths.h"
#include "core.h"
#include "array.h"

#ifndef ASSERT_UNIT_LENGTH
#define ASSERT_UNIT_LENGTH(v) do { ASSERT(almost_equal(length(v), 0, 0.0001f)); } while(0)
#endif

// -- Vector2
Vector2 abs(Vector2 v) EXPORT
{
    // TODO(jesper): cmath dependency
    return Vector2{ fabsf(v.x), fabsf(v.y) };
}

Vector2 vec2_max(Vector2 lhs, Vector2 rhs) EXPORT
{
    return { MAX(lhs.x, rhs.x), MAX(lhs.y, rhs.y) };
}

Vector2 calc_center(Vector2 tl, Vector2 br, Vector2 size) EXPORT
{
    return {
        calc_center(tl.x, br.x, size.x),
        calc_center(tl.y, br.y, size.y)
    };
}

Vector2 lerp(Vector2 a, Vector2 b, f32 t) EXPORT
{
    Vector2 r;
    r.x = lerp(a.x, b.x, t);
    r.y = lerp(a.y, b.y, t);
    return r;
}

f32 length(Vector2 v) EXPORT { return sqrtf(v.x*v.x + v.y*v.y); }
f32 length_sq(Vector2 v) EXPORT { return dot(v, v); }

Vector2 normalise(Vector2 v) EXPORT
{
    f32 length = sqrtf(v.x*v.x + v.y*v.y);
    Vector2 r;
    r.x = v.x / length;
    r.y = v.y / length;
    return r;
}

Vector2 tangent(Vector2 v) EXPORT { return { -v.y, v.x }; }
Vector2 line_normal(Vector2 start, Vector2 end) EXPORT
{
    Vector2 t = end - start;
    return normalise(tangent(t));
}

Vector2 operator-(Vector2 v) { return { -v.x, -v.y }; }

Vector2 operator+(Vector2 lhs, Vector2 rhs)
{
    Vector2 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    return r;
}

Vector2 operator+(Vector2 v, f32 scalar)
{
    Vector2 r;
    r.x = v.x + scalar;
    r.y = v.y + scalar;
    return r;
}

Vector2 operator-(Vector2 v, f32 scalar)
{
    Vector2 r;
    r.x = v.x - scalar;
    r.y = v.y - scalar;
    return r;
}

Vector2 operator+=(Vector2 &lhs, Vector2 rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2 operator*=(Vector2 &lhs, f32 rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
    Vector2 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    return r;
}

Vector2 operator-=(Vector2 &lhs, Vector2 rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 operator*(Vector2 v, f32 scalar)
{
    Vector2 r;
    r.x = v.x*scalar;
    r.y = v.y*scalar;
    return r;
}

Vector2 operator*(f32 scalar, Vector2 v)
{
    Vector2 r;
    r.x = v.x*scalar;
    r.y = v.y*scalar;
    return r;
}

Vector2 operator*(Vector2 lhs, Vector2 rhs)
{
    Vector2 v;
    v.x = lhs.x * rhs.x;
    v.y = lhs.y * rhs.y;
    return v;
}

Vector2 operator/(Vector2 v, f32 scalar)
{
    return { v.x / scalar, v.y / scalar };
}

Vector2 operator/(Vector2 lhs, Vector2 rhs)
{
    return { lhs.x / rhs.x, lhs.y / rhs.y };
}

bool operator==(const Vector2 &a, const Vector2 &b) { return a.x == b.x && a.y == b.y; }
bool operator!=(const Vector2 &a, const Vector2 &b) { return a.x != b.x || a.y != b.y; }

f32 dot(Vector2 lhs, Vector2 rhs) EXPORT { return lhs.x * rhs.x + lhs.y * rhs.y; }


// Vector3
Vector3 abs(Vector3 v) EXPORT
{
    // TODO(jesper): cmath dependency
    return Vector3{ fabsf(v.x), fabsf(v.y), fabsf(v.z ) };
}
f32 length(Vector3 v) EXPORT { return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }
f32 length_sq(Vector3 v) EXPORT { return dot(v, v); }

Vector3 lerp(Vector3 a, Vector3 b, f32 t) EXPORT
{
    Vector3 r;
    r.x = lerp(a.x, b.x, t);
    r.y = lerp(a.y, b.y, t);
    r.z = lerp(a.z, b.z, t);
    return r;
}

Vector3 normalise(Vector3 v) EXPORT
{
    f32 length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    Vector3 r;
    r.x = v.x / length;
    r.y = v.y / length;
    r.z = v.z / length;
    return r;
}

// projection of a onto b
// the components of a that are parallel to b
// project(a, b) = a∥b
// project(a, b) = length(a)*cos(theta)
Vector3 project(Vector3 a, Vector3 b) EXPORT { return b * (dot(a, b) / dot(b, b)); }
Vector3 project_unit(Vector3 a, Vector3 b) EXPORT
{
    ASSERT_UNIT_LENGTH(b);
    return b * dot(a, b);
}

// rejection of a from b
// remove the components of a that are parallel to b, resulting in the components that are perpendicular
// reject(a, b) = a⟂ b
// reject(a, b) = length(a)*sin(theta)
Vector3 reject(Vector3 a, Vector3 b)  EXPORT { return a - b * (dot(a, b) / dot(b, b)); }
Vector3 reject_unit(Vector3 a, Vector3 b) EXPORT
{
    ASSERT_UNIT_LENGTH(b);
    return a - b * dot(a,b);
}

Vector3 reflect(Vector3 v, Vector3 n) EXPORT { return v - 2.0f*dot(v, n)*n; }

Vector3 refract(Vector3 v, Vector3 n, f32 etai_over_etat) EXPORT
{
    f32 cos_theta = MIN(dot(-v, n), 1.0f);
    Vector3 d_perp = etai_over_etat * (v + cos_theta*n);
    Vector3 d_parallel = -sqrtf(fabs(1.0f-dot(d_perp, d_perp))) * n;
    return d_perp + d_parallel;
}

// Gram-Schmidt process
void orthogonalise(Vector3 *v0, Vector3 *v1, Vector3 *v2) EXPORT
{
    Vector3 u0 = *v0;
    Vector3 u1 = *v1 - project(*v1, u0);
    Vector3 u2 = *v2 - project(*v2, u0) - project(*v2, u1);

    *v0 = u0;
    *v1 = u1;
    *v2 = u2;
}

// Gram-Schmidt process
void orthonormalize(Vector3 *v0, Vector3 *v1, Vector3 *v2) EXPORT
{
    Vector3 u0 = *v0;
    Vector3 u1 = *v1 - project(*v1, u0);
    Vector3 u2 = *v2 - project(*v2, u0) - project(*v2, u1);

    *v0 = normalise(u0);
    *v1 = normalise(u1);
    *v2 = normalise(u2);
}

Vector3 operator-(Vector3 v) { return { -v.x, -v.y, -v.z }; }

Vector3 operator+(Vector3 lhs, Vector3 rhs)
{
    Vector3 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    r.z = lhs.z + rhs.z;
    return r;
}

Vector3 operator+(Vector3 v, f32 scalar)
{
    Vector3 r;
    r.x = v.x + scalar;
    r.y = v.y + scalar;
    r.z = v.z + scalar;
    return r;
}

Vector3 operator+=(Vector3 &lhs, f32 scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    return lhs;
}

Vector3 operator+=(Vector3 &lhs, Vector3 rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Vector3 operator-(Vector3 lhs, Vector3 rhs)
{
    Vector3 r;
    r.x = lhs.x-rhs.x;
    r.y = lhs.y-rhs.y;
    r.z = lhs.z-rhs.z;
    return r;
}

Vector3 operator-=(Vector3 &lhs, Vector3 rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Vector3 operator-(Vector3 v, f32 scalar)
{
    Vector3 r;
    r.x = v.x-scalar;
    r.y = v.y-scalar;
    r.z = v.z-scalar;
    return r;
}

Vector3 operator-=(Vector3 &lhs, f32 scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    return lhs;
}

Vector3 operator*(Vector3 v, f32 scalar)
{
    Vector3 r;
    r.x = scalar * v.x;
    r.y = scalar * v.y;
    r.z = scalar * v.z;
    return r;
}

Vector3 operator*=(Vector3 &v, f32 scalar)
{
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    return v;
}

Vector3 operator*(f32 scalar, Vector3 v)
{
    Vector3 r;
    r.x = scalar * v.x;
    r.y = scalar * v.y;
    r.z = scalar * v.z;
    return r;
}

Vector3 operator*(Vector3 lhs, Vector3 rhs)
{
    Vector3 v;
    v.x = lhs.x * rhs.x;
    v.y = lhs.y * rhs.y;
    v.z = lhs.z * rhs.z;
    return v;
}

Vector3 operator*=(Vector3 &lhs, Vector3 rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}

Vector3 operator/(Vector3 lhs, Vector3 rhs)
{
    Vector3 v;
    v.x = lhs.x / rhs.x;
    v.y = lhs.y / rhs.y;
    v.z = lhs.z / rhs.z;
    return v;
}

Vector3 operator/=(Vector3 &a, Vector3 b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    return a;
}

Vector3 operator/(Vector3 v, f32 s)
{
    Vector3 r;
    r.x = v.x/s;
    r.y = v.y/s;
    r.z = v.z/s;
    return r;
}

Vector3 operator/=(Vector3 &v, f32 s)
{
    v.x /= s;
    v.y /= s;
    v.z /= s;
    return v;
}

bool operator==(const Vector3 &a, const Vector3 &b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
bool operator!=(const Vector3 &a, const Vector3 &b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

f32 dot(Vector3 a, Vector3 b) EXPORT
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

f32 triple_prod(Vector3 a, Vector3 b, Vector3 c) EXPORT
{
    return dot(cross(a, b), c);
}

Vector3 cross(Vector3 a, Vector3 b) EXPORT
{
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x,
    };
}


// Vector4
f32 length(Vector4 v) EXPORT { return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w); }
f32 length_sq(Vector4 v) EXPORT { return dot(v, v); }

Vector4 lerp(Vector4 a, Vector4 b, f32 t) EXPORT
{
    Vector4 r;
    r.x = lerp(a.x, b.x, t);
    r.y = lerp(a.y, b.y, t);
    r.z = lerp(a.z, b.z, t);
    r.w = lerp(a.w, b.w, t);
    return r;
}

Vector4 normalise(Vector4 v) EXPORT
{
    f32 length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
    Vector4 r;
    r.x = v.x / length;
    r.y = v.y / length;
    r.z = v.z / length;
    r.w = v.w / length;
    return r;
}

Vector4 operator*(Vector4 v, f32 s)
{
    Vector4 r;
    r.x = v.x*s;
    r.y = v.y*s;
    r.z = v.z*s;
    r.w = v.w*s;
    return r;
}

Vector4 operator*(f32 s, Vector4 v)
{
    Vector4 r;
    r.x = v.x*s;
    r.y = v.y*s;
    r.z = v.z*s;
    r.w = v.w*s;
    return r;
}

Vector4 operator*(Vector4 a, Vector4 b)
{
    Vector4 r;
    r.x = a.x * b.x;
    r.y = a.y * b.y;
    r.z = a.z * b.z;
    r.w = a.w * b.w;
    return r;
}

Vector4 operator+(Vector4 a, Vector4 b)
{
    Vector4 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    r.w = a.w + b.w;
    return r;
}

Vector4 operator+(Vector4 v, f32 s)
{
    Vector4 r;
    r.x = v.x + s;
    r.y = v.y + s;
    r.z = v.z + s;
    r.w = v.w + s;
    return r;
}

Vector4 operator-(Vector4 a, Vector4 b)
{
    Vector4 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    r.w = a.w - b.w;
    return r;
}

Vector4 operator-(Vector4 v, f32 s)
{
    Vector4 r;
    r.x = v.x - s;
    r.y = v.y - s;
    r.z = v.z - s;
    r.w = v.w - s;
    return r;
}

Vector4 operator*(Matrix4 M, Vector4 v)
{
    return {
        M.m00*v.x + M.m01*v.y + M.m02*v.z + M.m03 * v.w,
        M.m10*v.x + M.m11*v.y + M.m12*v.z + M.m13 * v.w,
        M.m20*v.x + M.m21*v.y + M.m22*v.z + M.m23 * v.w,
        M.m30*v.x + M.m31*v.y + M.m32*v.z + M.m33 * v.w,
    };
}

Vector4 operator/(Vector4 a, Vector4 b)
{
    Vector4 r;
    r.x = a.x / b.x;
    r.y = a.y / b.y;
    r.z = a.z / b.z;
    r.w = a.w / b.w;
    return r;
}

Vector4 operator/(Vector4 v, f32 s)
{
    Vector4 r;
    r.x = v.x/s;
    r.y = v.y/s;
    r.z = v.z/s;
    r.w = v.w/s;
    return r;
}

bool operator==(const Vector4 &a, const Vector4 &b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
bool operator!=(const Vector4 &a, const Vector4 &b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }


f32 dot(Vector4 a, Vector4 b) EXPORT
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}


// Quaternion
f32 length(Quaternion q) EXPORT { return sqrtf(dot(q.xyzw, q.xyzw)); }
f32 length_sq(Quaternion q) EXPORT { return dot(q.xyzw, q.xyzw); }

Quaternion quat_identity() EXPORT { return { 0, 0, 0, 1 }; }

Quaternion quat_angle_axis(f32 theta, Vector3 v) EXPORT
{
    f32 half_theta = theta/2;
    f32 s = sinf(half_theta);
    return { v.x*s, v.y*s, v.z*s, cosf(half_theta) };
}

Quaternion quat_euler(f32 pitch, f32 yaw, f32 roll) EXPORT
{
    Quaternion q = quat_yaw(yaw) * quat_pitch(pitch) * quat_roll(roll);
    return normalise(q);
}

Quaternion quat_euler(Vector3 euler) EXPORT
{
    Quaternion q = quat_yaw(euler.y) * quat_pitch(euler.x) * quat_roll(euler.z);
    return normalise(q);
}

Quaternion quat_yaw(f32 theta) EXPORT { return quat_angle_axis(theta, { 0, 1, 0 }); }
Quaternion quat_pitch(f32 theta) EXPORT { return quat_angle_axis(theta, { 1, 0, 0 }); }
Quaternion quat_roll(f32 theta) EXPORT { return quat_angle_axis(theta, { 0, 0, 1 }); }

Quaternion quat_from_mat4(Matrix4 trs) EXPORT
{
    Quaternion q;
    f32 trace = trs.m00 + trs.m11 + trs.m22;
    if (trace > 0) {
        f32 s = 2*sqrtf(trace + 1.0f);
        q.w = 0.25f*s;
        q.x = (trs.m21 - trs.m12) / s;
        q.y = (trs.m02 - trs.m20) / s;
        q.z = (trs.m10 - trs.m01) / s;
    } else if (trs.m00 > trs.m11 && trs.m00 > trs.m22) {
        f32 s = 2.0f * sqrtf(1.0f + trs.m00 - trs.m11 - trs.m22);
        q.w = (trs.m21 - trs.m12) / s;
        q.x = 0.25f * s;
        q.y = (trs.m01 + trs.m10) / s;
        q.z = (trs.m02 + trs.m20) / s;
    } else if (trs.m11 > trs.m22) {
        f32 s = 2.0f * sqrtf(1.0f + trs.m11 - trs.m00 - trs.m22);
        q.w = (trs.m02 - trs.m20) / s;
        q.x = (trs.m01 + trs.m10) / s;
        q.y = 0.25f * s;
        q.z = (trs.m12 + trs.m21) / s;
    } else {
        f32 s = 2.0f * sqrtf(1.0f + trs.m22 - trs.m00 - trs.m11);
        q.w = (trs.m10 - trs.m01) / s;
        q.x = (trs.m02 + trs.m20) / s;
        q.y = (trs.m12 + trs.m21) / s;
        q.z = 0.25f * s;
    }

    return q;
}

Quaternion normalise(Quaternion q) EXPORT
{
    f32 length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    Quaternion r;
    r.x = q.x / length;
    r.y = q.y / length;
    r.z = q.z / length;
    r.w = q.w / length;
    return r;
}

Quaternion quat_conjugate(Quaternion q) EXPORT
{
    return { .xyz = -q.xyz, q.w };
}

Quaternion quat_inverse(Quaternion q) EXPORT
{
    return quat_conjugate(q) / dot(q.xyzw, q.xyzw);
}

Quaternion operator+(Quaternion p, Quaternion q) EXPORT
{
    return { p.x+q.x, p.y+q.y, p.z+q.z, p.w+q.w };
}

Quaternion operator-(Quaternion p, Vector3 v) EXPORT
{
    return { .xyz = p.xyz - v, p.w };
}

Quaternion operator*(Quaternion q1, Quaternion q2) EXPORT
{
    Quaternion r;
    r.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    r.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    r.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
    r.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    return r;
}

Vector3 operator*(Quaternion q, Vector3 v) EXPORT
{
    Vector3 t = 2.0f * cross(q.xyz, v);
    return v + q.w*t + cross(q.xyz, t);
}

Vector3 operator*(Vector3 v, Quaternion q) EXPORT
{
    Vector3 t = 2.0f * cross(q.xyz, v);
    return v + q.w*t + cross(q.xyz, t);
}

Quaternion operator*(Quaternion q, f32 scalar) EXPORT
{
    return { .v = q.v * scalar, q.s * scalar };
}

Quaternion operator*(f32 scalar, Quaternion q) EXPORT
{
    return { .v = q.v * scalar, q.s * scalar };
}

Quaternion operator/(Quaternion q, f32 scalar) EXPORT
{
    return { .xyzw = q.xyzw / scalar };
}

bool operator==(const Quaternion &a, const Quaternion &b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
bool operator!=(const Quaternion &a, const Quaternion &b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }



// Matrix3
Matrix3 mat3_identity() EXPORT
{
    Matrix3 r{};
    r.columns[0].data[0] = 1.0f;
    r.columns[1].data[1] = 1.0f;
    r.columns[2].data[2] = 1.0f;
    return r;
}

Matrix3 mat3_rows(Vector3 r0, Vector3 r1, Vector3 r2) EXPORT
{
    return { .columns = {
        { r0.x, r1.x, r2.x },
        { r0.y, r1.y, r2.y },
        { r0.z, r1.z, r2.z },
    }};
}

Matrix3 mat3_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 ratio /*=1*/) EXPORT
{
    Matrix3 M = mat3_identity();
    M[0][0] = 2 / (right-left);
    M[1][1] = (2 / (top-bottom)) * ratio;
    M[2][0] = -(right+left) / (right-left);
    M[2][1] = (-(top+bottom) / (top-bottom)) * ratio;
    return M;
}

Matrix3 mat3_rotate_x(f32 theta) EXPORT
{
    f32 cos_t = cosf(theta);
    f32 sin_t = sinf(theta);

    return { .columns = {
        { 1, 0,      0     },
        { 0,  cos_t, sin_t },
        { 0, -sin_t, cos_t }
    }};
}

Matrix3 mat3_rotate_y(f32 theta) EXPORT
{
    f32 cos_t = cosf(theta);
    f32 sin_t = sinf(theta);

    return { .columns = {
        { cos_t, 0, -sin_t },
        { 0,     1,  0     },
        { sin_t, 0,  cos_t },
    }};
}

Matrix3 mat3_rotate_z(f32 theta) EXPORT
{
    f32 cos_t = cosf(theta);
    f32 sin_t = sinf(theta);

    return { .columns = {
        {  cos_t, sin_t, 0 },
        { -sin_t, cos_t, 0 },
        {  0,      0,    1 },
    }};
}

Matrix3 mat3_rotate(Vector3 axis, f32 theta) EXPORT
{
    f32 cos_t = cosf(theta);
    f32 sin_t = sinf(theta);
    f32 d = 1 - cos_t;

    f32 x = axis.x*d;
    f32 y = axis.y*d;
    f32 z = axis.z*d;

    f32 axay = x*axis.y;
    f32 axaz = x*axis.z;
    f32 ayaz = y*axis.z;

    return { .columns = {
        { cos_t +     x*axis.x, axay  + sin_t*axis.z, axaz  - sin_t*axis.y },
        { axay  - sin_t*axis.z, cos_t +     y*axis.y, ayaz  + sin_t*axis.x },
        { axaz  + sin_t*axis.y, ayaz  - sin_t*axis.x, cos_t +     z*axis.z }
    }};
}

Matrix3 mat3_translate(Vector2 v) EXPORT
{
    Matrix3 M = mat3_identity();
    M[2].xy = v;
    return M;
}

Matrix3 mat3_scale(f32 scalar) EXPORT
{
    Matrix3 M{};
    M.m00 = scalar;
    M.m11 = scalar;
    M.m22 = scalar;
    return M;
}

Matrix3 mat3_scale_axis(Vector3 axis, f32 scalar) EXPORT
{
    scalar -= 1.0f;
    f32 x = axis.x*scalar;
    f32 y = axis.y*scalar;
    f32 z = axis.z*scalar;

    f32 axay = x*axis.y;
    f32 axaz = x*axis.z;
    f32 ayaz = y*axis.z;

    return mat3_rows(
        { x*axis.x + 1.0f, axay,            axaz },
        { axay,            y*axis.y + 1.0f, ayaz },
        { axaz,            ayaz,            z*axis.z + 1.0f });
}

Matrix3 mat3_skew(f32 t, Vector3 a, Vector3 b) EXPORT
{
    t = tanf(t);
    f32 x = a.x*t;
    f32 y = a.y*t;
    f32 z = a.z*t;

    return mat3_rows(
        { x*b.x + 1.0f, x*b.y,        x*b.z },
        { y*b.x,        y*b.y + 1.0f, y*b.z },
        { z*b.x,        z*b.y,        z*b.z + 1.0f });
}

Matrix3 mat3_reflect(Vector3 a) EXPORT
{
    f32 x = -2.0f*a.x;
    f32 y = -2.0f*a.y;
    f32 z = -2.0f*a.z;

    f32 axay = x*a.y;
    f32 axaz = x*a.z;
    f32 ayaz = y*a.z;

    return mat3_rows(
        { x*a.x + 1.0f, axay,         axaz },
        { axay,         y*a.y + 1.0f, ayaz },
        { axaz,         ayaz,         z*a.z + 1.0f });
}

Matrix3 mat3_transform(Matrix3 projection, Vector2 position) EXPORT
{
    Matrix3 view = mat3_translate(-position);
    return projection*view;
}

Matrix3 mat3_transform(Matrix3 projection, Vector2 position, f32 uni_scale) EXPORT
{
    Matrix3 mview = mat3_translate(-position);
    Matrix3 mscale = mat3_scale(uni_scale);
    return projection*mview*mscale;
}

Matrix3 mat3_transpose(Matrix3 m) EXPORT
{
    Matrix3 r;
    r[0][0] = m[0][0];
    r[0][1] = m[1][0];
    r[0][2] = m[2][0];

    r[1][0] = m[0][1];
    r[1][1] = m[1][1];
    r[1][2] = m[2][1];

    r[2][0] = m[0][2];
    r[2][1] = m[1][2];
    r[2][2] = m[2][2];
    return r;
}

Matrix3 mat3_inverse(Matrix3 M) EXPORT
{
    Vector3 c0 = M[0], c1 = M[1], c2 = M[2];
    Vector3 r0 = cross(c1, c2);
    Vector3 r1 = cross(c2, c0);
    Vector3 r2 = cross(c0, c1);

    f32 inv_det = 1 / dot(r2, c2);
    return { .columns = {
        { r0.x*inv_det, r1.x*inv_det, r2.x*inv_det },
        { r0.y*inv_det, r1.y*inv_det, r2.y*inv_det },
        { r0.z*inv_det, r1.z*inv_det, r2.z*inv_det },
    }};
}

f32 mat3_determinant(Matrix3 M) EXPORT
{
    return
        M.m00*M.m11*M.m22 - M.m12*M.m21 +
        M.m01*M.m12*M.m20 - M.m10*M.m22 +
        M.m02*M.m10*M.m21 - M.m11*M.m20;
}

Vector3 operator*(Matrix3 m, Vector3 v)
{
    Vector3 r;
    r[0] = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z;
    r[1] = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z;
    r[2] = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z;
    return r;
}

Matrix3 operator*(Matrix3 lhs, Matrix3 rhs)
{
    Matrix3 m{};
    m[0] = lhs[0]*rhs[0][0] + lhs[1]*rhs[0][1] + lhs[2]*rhs[0][2];
    m[1] = lhs[0]*rhs[1][0] + lhs[1]*rhs[1][1] + lhs[2]*rhs[1][2];
    m[2] = lhs[0]*rhs[2][0] + lhs[1]*rhs[2][1] + lhs[2]*rhs[2][2];
    return m;
}


// Matrix4
Matrix4 mat4_identity() EXPORT
{
    Matrix4 r{};
    r[0][0] = 1.0f;
    r[1][1] = 1.0f;
    r[2][2] = 1.0f;
    r[3][3] = 1.0f;
    return r;
}

Matrix4 mat4_transform_mat3(const Matrix3 &m0) EXPORT
{
    Matrix4 M = mat4_identity();
    M[0].xy = m0[0].xy;
    M[1].xy = m0[1].xy;
    M[3].xy = m0[2].xy;
    return M;
}

Matrix4 mat4_rotate(Quaternion q) EXPORT
{
    f32 xx = q.x*q.x;
    f32 yy = q.y*q.y;
    f32 zz = q.z*q.z;
    f32 xy = q.x*q.y;
    f32 xz = q.x*q.z;
    f32 yz = q.y*q.z;
    f32 wx = q.w*q.x;
    f32 wy = q.w*q.y;
    f32 wz = q.w*q.z;

    Matrix4 r;
    r[0][0] = 1 - 2*(yy + zz);
    r[0][1] = 2*(xy + wz);
    r[0][2] = 2*(xz - wy);
    r[0][3] = 0;

    r[1][0] = 2*(xy - wz);
    r[1][1] = 1 - 2*(xx + zz);
    r[1][2] = 2*(yz + wx);
    r[1][3] = 0;

    r[2][0] = 2*(xz + wy);
    r[2][1] = 2*(yz - wx);
    r[2][2] = 1 - 2*(xx + yy);
    r[2][3] = 0;

    r[3] = { 0, 0, 0, 1 };
    return r;
}

Matrix4 mat4_translate(Vector3 v) EXPORT
{
    Matrix4 r = mat4_identity();
    r[3] = { .xyz = v, 1 };
    return r;
}

Matrix4 mat4_scale(Vector3 v) EXPORT
{
    Matrix4 r = mat4_identity();
    r[0][0] = v.x;
    r[1][1] = v.y;
    r[2][2] = v.z;
    return r;
}

Matrix4 mat4_transform(Quaternion rotation, Vector3 position) EXPORT
{
    Matrix4 T = mat4_translate(position);
    Matrix4 R = mat4_rotate(rotation);
    return T*R;
}

Matrix4 mat4_transform(
    Quaternion rotation,
    Vector3 position,
    Vector3 scale) EXPORT
{
    Matrix4 T = mat4_translate(position);
    Matrix4 R = mat4_rotate(rotation);
    Matrix4 S = mat4_scale(scale);
    return T*R*S;
}

void mat4_trs_decompose(
    Matrix4 trs,
    Vector3 *pos,
    Quaternion *rot,
    Vector3 *scale) EXPORT
{
    *pos = trs[3].xyz;
    trs[3] = { 0, 0, 0, 1 };

    *scale = { length(trs[0].xyz), length(trs[1].xyz), length(trs[2].xyz) };
    trs[0] = { .xyz = trs[0].xyz / scale->x, 0 };
    trs[1] = { .xyz = trs[1].xyz / scale->y, 0 };
    trs[2] = { .xyz = trs[2].xyz / scale->z, 0 };

    *rot = quat_from_mat4(trs);
}

Matrix4 mat4_look_at(Vector3 eye, Vector3 center, Vector3 up) EXPORT
{
    Vector3 f = normalise(center - eye);
    Vector3 r = normalise(cross(f, up));
    Vector3 u = cross(r, f);

    Matrix4 M = mat4_identity();
    M[0].xyz = { r.x, u.x, -f.x };
    M[1].xyz = { r.y, u.y, -f.y };
    M[2].xyz = { r.z, u.z, -f.z };
    M[3].xyz = { -dot(r, eye), -dot(u, eye), dot(f, eye) };
    return M;
}

Matrix4 mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z) EXPORT
{
    // right-handed orthographic projection
    // zero-to-one depth range

    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = 1.0f / (far_z - near_z);

    Matrix4 M = mat4_identity();
    M[0].x = 2.0f * rl;
    M[1].y = 2.0f * tb;
    M[2].z = -1.0f * fn;
    M[3].xyz = { -(right+left) * rl, -(top+bottom) * tb, -near_z * fn };
    return M;
}

Matrix4 mat4_perspective(f32 fov, f32 aspect, f32 near_z, f32 far_z) EXPORT
{
    // right-handed perspective projection
    // zero-to-one depth range

    f32 tan_half_fov = tanf(fov/2);

    Matrix4 M{};
    M[0][0] = 1 / (aspect * tan_half_fov);
    M[1][1] = 1 / tan_half_fov;
    M[2][2] = far_z/(near_z-far_z);
    M[2][3] = -1;
    M[3][2] = -(far_z * near_z)/(far_z - near_z);
    return M;
}

Matrix4 mat4_inv_transform(Vector3 eye, Vector3 forward, Vector3 up) EXPORT
{
    Vector3 Z = normalise(forward);
    Vector3 X = normalise(cross(Z, up));
    Vector3 Y = cross(X, Z);

    Matrix4 M;
    M[0] = { X[0], Y[0], Z[0], 0 };
    M[1] = { X[1], Y[1], Z[1], 0 };
    M[2] = { X[2], Y[2], Z[2], 0 };
    M[3] = { -dot(X, eye), -dot(Y, eye), -dot(Z, eye), 1 };
    return M;
}

Matrix4 mat4_transpose(Matrix4 m0) EXPORT
{
    Matrix4 M;
    M[0][0] = m0[0][0];
    M[0][1] = m0[1][0];
    M[0][2] = m0[2][0];
    M[0][3] = m0[3][0];

    M[1][0] = m0[0][1];
    M[1][1] = m0[1][1];
    M[1][2] = m0[2][1];
    M[1][3] = m0[3][1];

    M[2][0] = m0[0][2];
    M[2][1] = m0[1][2];
    M[2][2] = m0[2][2];
    M[2][3] = m0[3][2];

    M[3][0] = m0[0][3];
    M[3][1] = m0[1][3];
    M[3][2] = m0[2][3];
    M[3][3] = m0[3][3];
    return M;
}

Matrix4 mat4_inverse(Matrix4 M) EXPORT
{
    Vector3 a = M[0].xyz, b = M[1].xyz, c = M[2].xyz, d = M[3].xyz;
    f32     x = M.m30,    y = M.m31,    z = M.m32,    w = M.m33;

    Vector3 s = cross(a, b);
    Vector3 t = cross(c, d);
    Vector3 u = a*y - b*x;
    Vector3 v = c*w - d*z;

    f32 inv_det = 1 / (dot(s,v) + dot(t, u));
    s *= inv_det;
    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

    Vector3 r0 = cross(b, v) + t*y;
    Vector3 r1 = cross(v, a) - t*x;
    Vector3 r2 = cross(d, u) + s*w;
    Vector3 r3 = cross(u, c) - s*z;

    return { .columns = {
        {  r0.x,      r1.x,       r2.x,      r3.x },
        {  r0.y,      r1.y,       r2.y,      r3.y },
        {  r0.z,      r1.z,       r2.z,      r3.z },
        { -dot(b, t), dot(a, t), -dot(d, s), dot(c, s) },
    }};
}

Matrix4 mat34_inverse(Matrix4 M) EXPORT
{
    Vector3 a = M[0].xyz, b = M[1].xyz, c = M[2].xyz, d = M[3].xyz;

    Vector3 s = cross(a, b);
    Vector3 t = cross(c, d);
    Vector3 v = c;

    f32 inv_det = 1.0f / dot(s, c);

    s *= inv_det;
    t *= inv_det;
    v *= inv_det;

    Vector3 r0 = cross(b, v);
    Vector3 r1 = cross(v, a);

    return { .columns = {
        {  r0.x,      r1.x,       s.x,       0 },
        {  r0.y,      r1.y,       s.y,       0 },
        {  r0.z,      r1.z,       s.z,       0 },
        { -dot(b, t), dot(a, t), -dot(d, s), 1 }
    }};
}

Matrix4 operator*(Matrix4 A, Matrix4 B) EXPORT
{
    Matrix4 M{};
    M.m00 = A.m00*B.m00 + A.m01*B.m10 + A.m02*B.m20 + A.m03*B.m30;
    M.m01 = A.m00*B.m01 + A.m01*B.m11 + A.m02*B.m21 + A.m03*B.m31;
    M.m02 = A.m00*B.m02 + A.m01*B.m12 + A.m02*B.m22 + A.m03*B.m32;
    M.m03 = A.m00*B.m03 + A.m01*B.m13 + A.m02*B.m23 + A.m03*B.m33;

    M.m10 = A.m10*B.m00 + A.m11*B.m10 + A.m12*B.m20 + A.m13*B.m30;
    M.m11 = A.m10*B.m01 + A.m11*B.m11 + A.m12*B.m21 + A.m13*B.m31;
    M.m12 = A.m10*B.m02 + A.m11*B.m12 + A.m12*B.m22 + A.m13*B.m32;
    M.m13 = A.m10*B.m03 + A.m11*B.m13 + A.m12*B.m23 + A.m13*B.m33;

    M.m20 = A.m20*B.m00 + A.m21*B.m10 + A.m22*B.m20 + A.m23*B.m30;
    M.m21 = A.m20*B.m01 + A.m21*B.m11 + A.m22*B.m21 + A.m23*B.m31;
    M.m22 = A.m20*B.m02 + A.m21*B.m12 + A.m22*B.m22 + A.m23*B.m32;
    M.m23 = A.m20*B.m03 + A.m21*B.m13 + A.m22*B.m23 + A.m23*B.m33;

    M.m30 = A.m30*B.m00 + A.m31*B.m10 + A.m32*B.m20 + A.m33*B.m30;
    M.m31 = A.m30*B.m01 + A.m31*B.m11 + A.m32*B.m21 + A.m33*B.m31;
    M.m32 = A.m30*B.m02 + A.m31*B.m12 + A.m32*B.m22 + A.m33*B.m32;
    M.m33 = A.m30*B.m03 + A.m31*B.m13 + A.m32*B.m23 + A.m33*B.m33;
    return M;
}

Matrix4 operator*(Matrix4 M, f32 s) EXPORT
{
    Matrix4 r;
    r[0] = M[0] * s;
    r[1] = M[1] * s;
    r[2] = M[2] * s;
    r[3] = M[3] * s;
    return r;
}


// intersection tests
bool ray_intersect_capsule(
    Vector3 ray_o, Vector3 ray_d,
    Vector3 cap_p0, Vector3 cap_p1, f32 cap_r,
    f32 *tr) EXPORT
{
    Vector3 ldir = cap_p1-cap_p0;
    Vector3 p = cap_p0-ray_o;
    f32 q = length_sq(ldir);
    f32 r = dot(ldir, ray_d);
    f32 s = dot(ldir, p);
    f32 t = dot(ray_d, p);
    f32 denom = q - r*r;

    f32 sn, sd, tn, td;
    if (denom < f32_EPSILON) {
        sd = td = 1.0f;
        sn = 0.0f;
        tn = t;
    } else {
        sd = td = denom;
        sn = r * t - s;
        tn = q * t - r * s;
        if (sn < 0.0f) {
            sn = 0.0f;
            tn = t;
            td = 1.0f;
        } else if (sn > sd) {
            sn = sd;
            tn = t + r;
            td = 1.0f;
        }
    }

    f32 ts;
    if (tn < 0.0f) {
        *tr = 0.0f;
        if (r >= 0.0f) {
            ts = 0.0f;
        } else if (s <= q) {
            ts = 1.0f;
        } else {
            ts = -s/q;
        }
    } else {
        *tr = tn/td;
        ts = sn/sd;
    }

    Vector3 nearest_p = cap_p0+ldir*ts;
    f32 dist_sq = length_sq(ray_o+ray_d*(*tr) - nearest_p);
    return dist_sq < cap_r*cap_r;
}

bool ray_intersect_triangle(
    Vector3 ray_o, Vector3 ray_d,
    Vector3 p0, Vector3 p1, Vector3 p2,
    f32 *tr) EXPORT
{
    Vector3 e1 = p1 - p0;
    Vector3 e2 = p2 - p0;
    Vector3 h = cross(ray_d, e2);

    f32 a = dot(e1, h);
    if (fabsf(a) < f32_EPSILON) return false;

    Vector3 s = ray_o - p0;

    f32 f = 1.0f / a;
    f32 u = f * dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;

    Vector3 q = cross(s, e1);
    f32 v = f * dot(ray_d, q);
    if (v < 0.0f || u + v > 1.0f) return false;

    f32 t = f * dot(e2, q);
    if (fabsf(t) > f32_EPSILON) {
        *tr = t;
        return true;
    }

    return false;
}

bool ray_intersect_quad(
    Vector3 ray_o, Vector3 ray_d,
    Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
    f32 *tr) EXPORT
{
    if (ray_intersect_triangle(ray_o, ray_d, p0, p1, p2, tr)) return true;
    if (ray_intersect_triangle(ray_o, ray_d, p2, p3, p0, tr)) return true;
    return false;
}

bool ray_intersect_plane(
    Vector3 ray_o, Vector3 ray_d,
    Vector3 plane_n, f32 plane_d,
    f32 *tr) EXPORT
{
    f32 d = dot(ray_d, plane_n);
    if (fabsf(d) < f32_EPSILON) {
        *tr = 0.0f;
        return false;
    }

    *tr = (plane_d - dot(ray_o, plane_n)) / d;
    return true;
}

bool nearest_ray_vs_line(
    Vector3 ray_o, Vector3 ray_d,
    Vector3 line_o, Vector3 line_d,
    f32 *t, f32 *u) EXPORT
{
    Vector3 v = ray_o - line_o;
    f32 q = dot(ray_d, line_d);
    f32 s = dot(line_d, v);

    f32 d = 1.0f - q*q;
    if (d < f32_EPSILON) { // parallel
        *t = 0.0f;
        *u = s;
        *t = MAX(*u, 0.0f);
        return false;
    } else {
        f32 r = dot(ray_d, v);
        *t = (q*s - r) / d;
        *u = (s - q*r) / d;
        *t = MAX(*t, 0.0f);
        return true;
    }
}

bool point_in_aabb(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size, f32 epsilon) EXPORT
{
    return
        p.x <= aabb_pos.x + aabb_half_size.x + epsilon &&
        p.x >= aabb_pos.x - aabb_half_size.x - epsilon &&
        p.y <= aabb_pos.y + aabb_half_size.y + epsilon &&
        p.y >= aabb_pos.y - aabb_half_size.y - epsilon;
}

bool point_in_circle(Vector2 p, Vector2 c, f32 radius_sq) EXPORT
{
    Vector2 pc = p - c;
    return dot(pc, pc) <= radius_sq;
}

Vector2 point_clamp_aabb(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size) EXPORT
{
    Vector2 r;
    r.x = CLAMP(
        p.x,
        aabb_pos.x - aabb_half_size.x,
        aabb_pos.x + aabb_half_size.x);
    r.y = CLAMP(
        p.y,
        aabb_pos.y - aabb_half_size.y,
        aabb_pos.y + aabb_half_size.y);
    return r;
}


Vector2 point_clamp_aabb_circle(
    Vector2 p,
    Vector2 aabb_pos,
    Vector2 aabb_half_size,
    Vector2 c_p,
    f32 c_r) EXPORT
{
    Vector2 r = point_clamp_aabb(p, aabb_pos, aabb_half_size);
    r = c_p + c_r * normalise(r - c_p);
    r = point_clamp_aabb(p, aabb_pos, aabb_half_size);

    return r;
}

bool point_in_triangle(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2) EXPORT
{
    f32 area = 0.5f * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
    f32 sign = area < 0.0f ? -1.0f: 1.0f;
    f32 s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
    f32 t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;
    return s > 0.0f && t > 0.0f && (s + t) < 2.0f * area * sign;
}

bool point_in_rect(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3) EXPORT
{
    f32 left = MIN4(p0.x, p1.x, p2.x, p3.x);
    f32 right = MAX4(p0.x, p1.x, p2.x, p3.x);
    f32 top = MIN4(p0.y, p1.y, p2.y, p3.y);
    f32 bot = MAX4(p0.y, p1.y, p2.y, p3.y);

    if (p.x < left || p.x > right || p.y < top || p.y > bot) return false;
    return true;
}

bool point_in_rect(Vector2 p, Vector2 center, Vector2 size) EXPORT
{
    Vector2 hs = 0.5f*size;
    Vector2 tl = center - hs;
    Vector2 br = center + hs;
    Vector2 tr{ br.x, tl.y };
    Vector2 bl{ tl.x, br.y };
    return point_in_rect(p, tl, tr, br, bl);
}

bool point_in_rect(Vector2 p, Rect r) EXPORT
{
    return
        p.x >= r.tl.x && p.x <= r.br.x &&
        p.y >= r.tl.y && p.y <= r.br.y;
}

Contact aabb_intersect_aabb(
    Vector2 pos_a,
    Vector2 extents_a,
    Vector2 pos_b,
    Vector2 extents_b) EXPORT
{
    Vector2 dv = pos_b - pos_a;
    Vector2 pv = extents_b + extents_a - abs(dv);

    if (pv.x <= 0.0f || pv.y <= 0.0f) return INVALID_CONTACT;

    if (pv.x < pv.y) {
        f32 sign = dv.x > 0 ? 1 : -1;

        return {
            .point  = pos_a + sign*Vector2{ pv.x, 0.0f },
            .normal = { sign, 0.0f },
            .depth  = pv.x
        };
    } else {
        f32 sign = dv.y > 0 ? 1 : -1;
        return {
            .point  = pos_a + sign*Vector2{ 0.0f, pv.y },
            .normal = { 0.0f, sign },
            .depth  = pv.y
        };
    }
}

Contact aabb_intersect_line(
    Vector2 pos_a,
    Vector2 extents_a,
    Vector2 l_p0,
    Vector2 l_p1) EXPORT
{
    FixedArray<Line2, 4> aabb_lines; aabb_lines.count = 4;
    calc_aabb_lines(aabb_lines.data, pos_a, extents_a);

    Contact contact = INVALID_CONTACT;
    for (auto l : aabb_lines) {
        if (Vector2 p;
            line_intersect_line(
                l.p0, l.p1,
                l_p0, l_p1,
                &p))
        {
            //Vector2 aabb_n = line_normal(l.p0, l.p1);
            Vector2 line_n = line_normal(l_p0, l_p1);

            f32 depth = dot(p - pos_a, line_n);

            if (depth < contact.depth) {
                contact = {
                    .point  = p,
                    .normal = line_n,
                    .depth  = depth
                };
            }
        }
    }

    return contact ;
}

bool line_intersect_line(Line2 l0, Line2 l1, Vector2 *intersect_point) EXPORT
{
    return line_intersect_line(l0.p0, l0.p1, l1.p0, l1.p1, intersect_point);
}

bool line_intersect_line(
    Vector2 p0, Vector2 p1,
    Vector2 p2, Vector2 p3,
    Vector2 *intersect_point) EXPORT
{
    Vector2 s1 = p1 - p0;
    Vector2 s2 = p3 - p2;

    Vector2 u = p0 - p2;

    f32 denom = (-s2.x * s1.y + s1.x * s2.y);
    f32 ip = 1.0f / denom;

    f32 s = (-s1.y * u.x + s1.x * u.y) * ip;
    f32 t = ( s2.x * u.y - s2.y * u.x) * ip;

    if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) {
        if (intersect_point) *intersect_point = p0 + (s1 * t);
        return true;
    }

    return false;
}

bool line_intersect_aabb(
    Vector2 l_p0,
    Vector2 l_p1,
    Vector2 pos,
    Vector2 half_size,
    Vector2 *out_intersect_point,
    Vector2 *out_normal) EXPORT
{
    f32 min_length_sq = f32_MAX;
    Vector2 intersect_point;
    Vector2 normal;

    using Line = Vector2[2];

    Line lines[] = {
        { pos.x + half_size.x, pos.y - half_size.y },
        { pos.x - half_size.x, pos.y - half_size.y },
        { pos.x - half_size.x, pos.y + half_size.y },
        { pos.x + half_size.x, pos.y + half_size.y }
    };

    for (i32 i = 0; i < ARRAY_COUNT(lines); i++) {
        Vector2 p0 = lines[i][0];
        Vector2 p1 = lines[i][1];

        Vector2 p;
        if (line_intersect_line(l_p0, l_p1, p0, p1, &p)) {
            f32 ll = length_sq(p-pos);
            if (ll < min_length_sq) {
                min_length_sq = ll;
                intersect_point = p;

                Vector2 p0p1 = p1 - p0;
                normal = { p0p1.y, -p0p1.x };
            }
        }
    }

    if (min_length_sq != f32_MAX) {
        if (out_intersect_point) *out_intersect_point = intersect_point;
        if (out_normal) *out_normal = normal;
        return true;
    }

    return false;
}

void calc_aabb_lines(Line2 lines[4], Vector2 pos, Vector2 extents) EXPORT
{
    lines[0] = { { pos.x + extents.x, pos.y - extents.y }, { pos.x - extents.x, pos.y - extents.y } };
    lines[1] = { { pos.x - extents.x, pos.y - extents.y }, { pos.x - extents.x, pos.y + extents.y } };
    lines[2] = { { pos.x - extents.x, pos.y + extents.y }, { pos.x + extents.x, pos.y + extents.y } };
    lines[3] = { { pos.x + extents.x, pos.y + extents.y }, { pos.x + extents.x, pos.y - extents.y } };
}

bool aabb_intersect_circle(
    Vector2 aabb_p,
    Vector2 aabb_half_size,
    Vector2 circle_p,
    f32 circle_r) EXPORT
{
    Vector2 closest = point_clamp_aabb(circle_p, aabb_p, aabb_half_size);
    return length_sq(closest - circle_p) < circle_r*circle_r;
}


bool aabb_intersect_aabb(
    Vector2 pos_a,
    Vector2 extents_a,
    Vector2 pos_b,
    Vector2 extents_b,
    Vector2 *out_delta,
    Vector2 *out_normal) EXPORT
{
    Vector2 dv = pos_b - pos_a;
    Vector2 pv = extents_b + extents_a - abs(dv);

    if (pv.x <= 0.0f || pv.y <= 0.0f) return false;

    if (pv.x < pv.y) {
        f32 sign = dv.x > 0 ? 1 : -1;
        if (out_delta) *out_delta = sign*Vector2{ pv.x, 0.0f };
        if (out_normal) *out_normal = { sign, 0.0f };
    } else {
        f32 sign = dv.y > 0 ? 1 : -1;
        if (out_delta) *out_delta = sign*Vector2{ 0.0f, pv.y };
        if (out_normal) *out_normal = { 0.0f, sign };
    }

    return true;
}

bool aabb_intersect_line(
    Vector2 pos,
    Vector2 extents,
    Vector2 l_p0,
    Vector2 l_p1,
    Vector2 *out_intersect_point,
    Vector2 *out_normal) EXPORT
{
    FixedArray<Line2, 4> aabb_lines; aabb_lines.count = 4;
    calc_aabb_lines(aabb_lines.data, pos, extents);

    for (auto l : aabb_lines) {
        if (line_intersect_line(
                l.p0, l.p1,
                l_p0, l_p1,
                out_intersect_point))
        {
            if (out_normal) *out_normal = line_normal(l.p0, l.p1);
            return true;
        }
    }

    return false;
}

bool swept_aabb_intersect_line(
    Vector2 pos,
    Vector2 half_size,
    Vector2 delta,
    Vector2 l_p0,
    Vector2 l_p1,
    Vector2 *out_intersect_point,
    Vector2 *out_normal) EXPORT
{
    if (l_p0.y > l_p1.y) SWAP(l_p0, l_p1);

    Line2 ls0[] = {
        {
            { l_p0.x - half_size.x, l_p0.y - half_size.y },
            { l_p0.x + half_size.x, l_p0.y - half_size.y },
        },
        {
            { l_p0.x + half_size.x, l_p0.y - half_size.y },
            { l_p1.x + half_size.x, l_p1.y - half_size.y },
        },
        {
            { l_p1.x + half_size.x, l_p1.y - half_size.y },
            { l_p1.x + half_size.x, l_p1.y + half_size.y },
        },
        {
            { l_p1.x + half_size.x, l_p1.y + half_size.y },
            { l_p1.x - half_size.x, l_p1.y + half_size.y },
        },
        {
            { l_p1.x - half_size.x, l_p1.y + half_size.y },
            { l_p0.x - half_size.x, l_p0.y + half_size.y },
        },
        {
            { l_p0.x - half_size.x, l_p0.y + half_size.y },
            { l_p0.x - half_size.x, l_p0.y - half_size.y },
        }
    };

    Line2 ls1[] = {
        {
            { l_p0.x - half_size.x, l_p0.y - half_size.y },
            { l_p0.x + half_size.x, l_p0.y - half_size.y },
        },
        {
            { l_p0.x + half_size.x, l_p0.y - half_size.y },
            { l_p0.x + half_size.x, l_p0.y + half_size.y },
        },
        {
            { l_p0.x + half_size.x, l_p0.y + half_size.y },
            { l_p1.x + half_size.x, l_p1.y + half_size.y },
        },
        {
            { l_p1.x + half_size.x, l_p1.y + half_size.y },
            { l_p1.x - half_size.x, l_p1.y + half_size.y },
        },
        {
            { l_p1.x - half_size.x, l_p1.y + half_size.y },
            { l_p1.x - half_size.x, l_p1.y - half_size.y },
        },
        {
            { l_p1.x - half_size.x, l_p1.y - half_size.y },
            { l_p0.x - half_size.x, l_p0.y - half_size.y },
        }
    };

    i32 line_count = l_p0.x < l_p1.x ? ARRAY_COUNT(ls0) : ARRAY_COUNT(ls1);
    Line2 *lines = l_p0.x < l_p1.x ? ls0 : ls1;

    f32 min_length_sq = f32_MAX;
    Vector2 intersect_point;
    Vector2 normal;

    for (i32 i = 0; i < line_count; i++) {
        Vector2 p0 = lines[i].p0;
        Vector2 p1 = lines[i].p1;

        Vector2 p;
        if (line_intersect_line(p0, p1, pos, pos + delta, &p)) {
            f32 ll = dot(p, pos);
            if (ll < min_length_sq) {
                min_length_sq = ll;
                intersect_point = p;

                Vector2 p0p1 = p1 - p0;
                normal = { p0p1.y, -p0p1.x };
            }
        }
    }

    if (min_length_sq != f32_MAX) {
        if (out_intersect_point) *out_intersect_point = intersect_point;
        if (out_normal) *out_normal = normal;
        return true;
    }

    return false;
}

bool aabb_intersect_swept_aabb(
    Vector2 pos_a,
    Vector2 half_size_a,
    Vector2 pos_b,
    Vector2 half_size_b,
    Vector2 delta_b,
    Vector2 *out_intersect_point,
    Vector2 *out_normal) EXPORT
{
    Vector2 half_size = half_size_a + half_size_b;

    Vector2 minkowski_lines[][2] = {
        { // top
            { pos_a.x - half_size.x, pos_a.y - half_size.y },
            { pos_a.x + half_size.x, pos_a.y - half_size.y },
        },
        { // right
            { pos_a.x + half_size.x, pos_a.y - half_size.y },
            { pos_a.x + half_size.x, pos_a.y + half_size.y }
        },
        { // bottom
            { pos_a.x + half_size.x, pos_a.y + half_size.y },
            { pos_a.x - half_size.x, pos_a.y + half_size.y }
        },
        { // left
            { pos_a.x - half_size.x, pos_a.y + half_size.y },
            { pos_a.x - half_size.x, pos_a.y - half_size.y }
        }
    };

    f32 min_length_sq = f32_MAX;
    Vector2 intersect_point;
    Vector2 normal;
    for (i32 i = 0; i < ARRAY_COUNT(minkowski_lines); i++) {
        Vector2 p0 = minkowski_lines[i][0];
        Vector2 p1 = minkowski_lines[i][1];

        Vector2 p;
        if (line_intersect_line(p0, p1, pos_b, pos_b + delta_b, &p)) {
            f32 ll = dot(p, pos_b);
            if (ll < min_length_sq) {
                min_length_sq = ll;
                intersect_point = p;

                Vector2 p0p1 = p1-p0;
                normal = { p0p1.y, -p0p1.x };
            }
        }
    }

    if (min_length_sq != f32_MAX) {
        if (out_intersect_point) *out_intersect_point = intersect_point;
        if (out_normal) *out_normal = normal;
        return true;
    }
    return false;
}


f32 calc_center(f32 min, f32 max, f32 size) EXPORT
{
    return min + 0.5f*(max-min - size);
}


f32 round_to(f32 value, f32 multiple) EXPORT
{
    f32 hm = 0.5f*multiple;
    f32 sign = value < 0.0f ? -1.0f : 1.0f;
    f32 abs_value = fabsf(value);
    return sign*((abs_value+hm) - fmodf(abs_value+hm, multiple));
}

Vector2 round_to(Vector2 v, f32 multiple) EXPORT
{
    return { round_to(v.x, multiple), round_to(v.y, multiple) };
}

f32 lerp(f32 a, f32 b, f32 t) EXPORT
{
    return (1.0f-t) * a + t*b;
}


u32 rand_u32(XORShift128 *series) EXPORT
{
    u32 t = series->state[3];
    u32 s = series->state[0];
    series->state[3] = series->state[2];
    series->state[2] = series->state[1];
    series->state[1] = s;

    t ^= t << 11;
    t ^= t >> 8;
    series->state[0] = t ^ s ^ (s >> 19);
    return series->state[0];
}

f32 rand_f32(XORShift128 *series) EXPORT
{
    u32 r = rand_u32(series);
    return (f32)(r >> 1) / (f32)(u32_MAX >> 1);
}

f32 rand_f32(XORShift128 *series, f32 min, f32 max) EXPORT
{
    return min + (max-min)*rand_f32(series);
}

Vector3 rand_sphere(XORShift128 *series) EXPORT
{
    Vector3 v;
    for (;;) {
        v.x = rand_f32(series, -1.0f, 1.0f);
        v.y = rand_f32(series, -1.0f, 1.0f);
        v.z = rand_f32(series, -1.0f, 1.0f);
        if (dot(v, v) < 1.0f) return v;
    }
}

Vector3 rand_color3(XORShift128 *series) EXPORT
{
    Vector3 v;
    v.x = rand_f32(series);
    v.y = rand_f32(series);
    v.z = rand_f32(series);
    return v;
}

Vector3 rand_hemisphere(Vector3 normal, XORShift128 *series) EXPORT
{
    Vector3 s = rand_sphere(series);
    if (dot(s, normal) > 0.0f) return s;
    return -s;
}

Vector3 rand_disc(XORShift128 *series) EXPORT
{
    for (;;) {
        Vector3 v;
        v.x = rand_f32(series, -1.0f, 1.0f);
        v.y = rand_f32(series, -1.0f, 1.0f);
        v.z = 0.0f;
        if (dot(v, v) < 1.0) return v;
    }
}

f32 ray_hit_sphere(Vector3 ray_o, Vector3 ray_d, Vector3 sphere_p, f32 sphere_r)
{
    Vector3 oc = ray_o - sphere_p;
    f32 a = dot(ray_d, ray_d);
    f32 half_b = dot(oc, ray_d);
    f32 c = dot(oc, oc) - sphere_r*sphere_r;
    f32 disc = half_b*half_b - a*c;

    if (disc < 0.0f) return -1.0f;
    return (-half_b - sqrtf(disc)) / a;
}

f32 sRGB_from_linear(f32 l)
{
    if (l > 1.0f) return 1.0f;
    else if (l < 0.0f) return 0.0f;
    return l > 0.0031308f ? 1.055f*powf(l, 1.0f/2.4f) - 0.055f : l*12.92f;
}

Vector3 sRGB_from_linear(Vector3 l)
{
    Vector3 s;
    s.r = sRGB_from_linear(l.r);
    s.g = sRGB_from_linear(l.g);
    s.b = sRGB_from_linear(l.b);
    return s;
}



f32 reflectance(f32 cosine, f32 ref_idx)
{
    // Schlick's approximation
    f32 r0 = (1.0f-ref_idx) / (1.0f+ref_idx);
    r0 = r0*r0;
    return r0 + (1.0f-r0)*powf((1.0f-cosine), 5);
}

f32 radf(f32 theta) EXPORT
{
    return theta * f32_PI / 180.0f;
}

f32 degf(f32 rad) EXPORT
{
    return rad * 180.0f / f32_PI;
}

bool almost_equal(f32 a, f32 b, f32 epsilon /*=1e-6f*/) EXPORT
{
    return fabsf(a-b) < epsilon;
}

bool almost_equal(Vector3 a, Vector3 b, f32 epsilon /*=1e-6f*/) EXPORT
{
    return almost_equal(a.x, b.x, epsilon) &&
        almost_equal(a.y, b.y, epsilon) &&
        almost_equal(a.z, b.z, epsilon);
}

bool almost_equal(Quaternion q1, Quaternion q2, f32 epsilon /*=1e-6f*/) EXPORT
{
    return almost_equal(q1.x, q2.x, epsilon) &&
        almost_equal(q1.y, q2.y, epsilon) &&
        almost_equal(q1.z, q2.z, epsilon);
        almost_equal(q1.w, q2.w, epsilon);
}

bool almost_equal(Matrix3 A, Matrix3 B, f32 epsilon /*=1e-6f*/) EXPORT
{
    for (i32 i = 0; i < ARRAY_COUNT(A.data); i++) {
        if (!almost_equal(A.data[i], B.data[i], epsilon)) return false;
    }


    return true;
}

bool almost_equal(Matrix4 A, Matrix4 B, f32 epsilon) EXPORT
{
    for (i32 i = 0; i < ARRAY_COUNT(A.data); i++) {
        if (!almost_equal(A.data[i], B.data[i], epsilon)) return false;
    }

    return true;
}



f32 angle_between(Vector3 v0, Vector3 v1) EXPORT
{
    f32 denom = length(v0) * length(v1);
    f32 theta = acosf(dot(v0, v1) / denom);
    return theta;
}




bool operator==(const Rect &lhs, const Rect &rhs)
{
    return lhs.tl == rhs.tl && lhs.br == rhs.br;
}


bool operator!=(const Rect &lhs, const Rect &rhs)
{
    return lhs.tl != rhs.tl || lhs.br != rhs.br;
}

Vector3 rgb_mul(Vector3 rgb, f32 v)
{
    return {
        CLAMP(rgb.x * v, 0, 1),
        CLAMP(rgb.y * v, 0, 1),
        CLAMP(rgb.z * v, 0, 1)
   };
}

u32 bgra_pack(f32 r, f32 g, f32 b, f32 a)
{
    return u32(b*255.0f) | u32(g*255.0f) << 8 | u32(r*255.0f) << 16 | u32(a*255.0f) << 24;
}

u32 rgb_pack(Vector3 c)
{
    return u32(c.r*255.0f) | u32(c.g*255.0f) << 8 | u32(c.b*255.0f) << 16;

}

u32 bgr_pack(Vector3 c)
{
    return u32(c.b*255.0f) | u32(c.g*255.0f) << 8 | u32(c.r*255.0f) << 16;

}

Vector3 bgr_unpack(u32 argb)
{
    Vector3 rgb;
    rgb.b = ((argb >> 0) & 0xFF) / 255.0f;
    rgb.g = ((argb >> 8) & 0xFF) / 255.0f;
    rgb.r = ((argb >> 16) & 0xFF) / 255.0f;
    return rgb;
}

Vector4 bgra_unpack(u32 argb)
{
    Vector4 v;
    v.b = ((argb >> 0) & 0xFF) / 255.0f;
    v.g = ((argb >> 8) & 0xFF) / 255.0f;
    v.r = ((argb >> 16) & 0xFF) / 255.0f;
    v.a = ((argb >> 24) & 0xFF) / 255.0f;
    return v;
}

f32 rgb_from_hue(f32 p, f32 q, f32 t)
{
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (6*t < 1.0f) return p + (q - p) * 6 * t;
    if (2*t < 1.0f) return q;
    if (3*t < 2.0f) return p + (q - p) * (2.0f/3.0f - t) * 6;
    return p;
}

Vector3 rgb_from_hsl(f32 h, f32 s, f32 l)
{
    Vector3 rgb;

    if (s == 0.0f) {
        rgb.r = rgb.g = rgb.b = l;
    } else {
        f32 q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        f32 p = 2 * l - q;

        rgb.r = rgb_from_hue(p, q, h + 1.0f/3.0f);
        rgb.g = rgb_from_hue(p, q, h);
        rgb.b = rgb_from_hue(p, q, h - 1.0f/3.0f);
    }

    return rgb;
}

Vector3 rgb_unpack(u32 argb) EXPORT
{
    Vector3 rgb;
    rgb.r = ((argb >> 16) & 0xFF) / 255.0f;
    rgb.g = ((argb >> 8) & 0xFF) / 255.0f;
    rgb.b = ((argb >> 0) & 0xFF) / 255.0f;
    return rgb;
}

Vector4 argb_unpack(u32 argb) EXPORT
{
    Vector4 v;
    v.r = ((argb >> 16) & 0xFF) / 255.0f;
    v.g = ((argb >> 8) & 0xFF) / 255.0f;
    v.b = ((argb >> 0) & 0xFF) / 255.0f;
    v.a = ((argb >> 24) & 0xFF) / 255.0f;
    return v;
}

f32 linear_from_sRGB(f32 s) EXPORT
{
    if (s <= 0.04045f) {
        return s / 12.92f;
    } else {
        return powf((s + 0.055f) / (1.055f), 2.4f);
    }
}

Vector3 linear_from_sRGB(Vector3 sRGB) EXPORT
{
    Vector3 l;
    l.r = linear_from_sRGB(sRGB.r);
    l.g = linear_from_sRGB(sRGB.g);
    l.b = linear_from_sRGB(sRGB.b);
    return l;
}

Vector4 linear_from_sRGB(Vector4 sRGB) EXPORT
{
    Vector4 l;
    l.r = linear_from_sRGB(sRGB.r);
    l.g = linear_from_sRGB(sRGB.g);
    l.b = linear_from_sRGB(sRGB.b);
    l.a = sRGB.a;
    return l;
}


/// test suite
#include "test.h"
TEST_PROC(maths__vector2__operators)
{
    {
        Vector2 a{ 1, 2 };
        Vector2 b{ 5, 7 };
        Vector2 c = a + b;
        ASSERT(c.x == 6);
        ASSERT(c.y == 9);
    }
    {
        Vector2 a{ 1, 2 };
        f32 s = 5;
        Vector2 c = a + s;
        ASSERT(c.x == 6);
        ASSERT(c.y == 7);
    }

    {
        Vector2 a{ 1, 2 };
        Vector2 b{ 5, 7 };
        Vector2 c = a - b;
        ASSERT(c.x == -4);
        ASSERT(c.y == -5);
    }
    {
        Vector2 a{ 1, 2 };
        f32 s = 5;
        Vector2 c = a - s;
        ASSERT(c.x == -4);
        ASSERT(c.y == -3);
    }

    {
        Vector2 a{ 1, 2 };
        Vector2 b{ 5, 7 };
        Vector2 c = a * b;
        ASSERT(c.x == 5);
        ASSERT(c.y == 14);
    }
    {
        Vector2 a{ 1, 2 };
        f32 s = 5;
        Vector2 c = a * s;
        ASSERT(c.x == 5);
        ASSERT(c.y == 10);
    }

    {
        Vector2 a{ 1, 2 };
        Vector2 b{ 8, 16 };
        Vector2 c = a / b;
        ASSERT(c.x == 0.125f);
        ASSERT(c.y == 0.125f);
    }
    {
        Vector2 a{ 1, 2 };
        f32 s = 8;
        Vector2 c = a / s;
        ASSERT(c.x == 0.125f);
        ASSERT(c.y == 0.25f);
    }
}

TEST_PROC(maths__vector2__swizzle)
{
    {
        Vector2 a{ 1, 2 };
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);

        ASSERT(a.x == 1);
        ASSERT(a.y == 2);

        ASSERT(a.w == 1);
        ASSERT(a.h == 2);

        ASSERT(a.r == 1);
        ASSERT(a.g == 2);

        ASSERT(a.u == 1);
        ASSERT(a.v == 2);
    }
}

TEST_PROC(maths__vector3__operators)
{
    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        Vector3 c = a + b;
        ASSERT(c.x == 6);
        ASSERT(c.y == 9);
        ASSERT(c.z == 14);
    }
    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        a += b;
        ASSERT(a.x == 6);
        ASSERT(a.y == 9);
        ASSERT(a.z == 14);
    }

    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        Vector3 c = a + s;
        ASSERT(c.x == 6);
        ASSERT(c.y == 7);
        ASSERT(c.z == 8);
    }
    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        a += s;
        ASSERT(a.x == 6);
        ASSERT(a.y == 7);
        ASSERT(a.z == 8);
    }

    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        Vector3 c = a - b;
        ASSERT(c.x == -4);
        ASSERT(c.y == -5);
        ASSERT(c.z == -8);
    }
    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        a -= b;
        ASSERT(a.x == -4);
        ASSERT(a.y == -5);
        ASSERT(a.z == -8);
    }

    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        Vector3 c = a - s;
        ASSERT(c.x == -4);
        ASSERT(c.y == -3);
        ASSERT(c.z == -2);
    }
    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        a -= s;
        ASSERT(a.x == -4);
        ASSERT(a.y == -3);
        ASSERT(a.z == -2);
    }

    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        Vector3 c = a * s;
        ASSERT(c.x == 5);
        ASSERT(c.y == 10);
        ASSERT(c.z == 15);
    }
    {
        Vector3 a{ 1, 2, 3 };
        f32 s = 5;
        a *= s;
        ASSERT(a.x == 5);
        ASSERT(a.y == 10);
        ASSERT(a.z == 15);
    }

    {
        Vector3 a{ 1, 2, 4 };
        f32 s = 8;
        Vector3 c = a / s;
        ASSERT(c.x == 0.125f);
        ASSERT(c.y == 0.25f);
        ASSERT(c.z == 0.5f);
    }
    {
        Vector3 a{ 1, 2, 4 };
        f32 s = 8;
        a /= s;
        ASSERT(a.x == 0.125f);
        ASSERT(a.y == 0.25f);
        ASSERT(a.z == 0.5f);
    }
}

TEST_PROC(maths__vector3__vec_mul_vec_equals_component_wise_mul)
{
    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        Vector3 c = a * b;
        ASSERT(c.x == 5);
        ASSERT(c.y == 14);
        ASSERT(c.z == 33);
    }
    {
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 5, 7, 11 };
        a *= b;
        ASSERT(a.x == 5);
        ASSERT(a.y == 14);
        ASSERT(a.z == 33);
    }

}

TEST_PROC(maths__vector3__vec_div_vec_equals_component_wise_div)
{
    {
        Vector3 a{ 1, 2, 4 };
        Vector3 b{ 8, 16, 32};
        Vector3 c = a / b;
        ASSERT(c.x == 0.125f);
        ASSERT(c.y == 0.125f);
        ASSERT(c.z == 0.125f);
    }
    {
        Vector3 a{ 1, 2, 4 };
        Vector3 b{ 8, 16, 32};
        a /= b;
        ASSERT(a.x == 0.125f);
        ASSERT(a.y == 0.125f);
        ASSERT(a.z == 0.125f);
    }
}


TEST_PROC(maths__vector3__swizzle)
{
    {
        Vector3 a{ 1, 2, 3 };
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);

        ASSERT(a.x == 1);
        ASSERT(a.y == 2);
        ASSERT(a.z == 3);

        ASSERT(a.w == 1);
        ASSERT(a.h == 2);
        ASSERT(a.d == 3);

        ASSERT(a.r == 1);
        ASSERT(a.g == 2);
        ASSERT(a.b == 3);

        ASSERT(a.xy == Vector2{ 1, 2 });
        ASSERT(a.yz == Vector2{ 2, 3 });

        ASSERT(a.wh == Vector2{ 1, 2 });
        ASSERT(a.hd == Vector2{ 2, 3 });

        ASSERT(a.rg == Vector2{ 1, 2 });
        ASSERT(a.gb == Vector2{ 2, 3 });
    }
}

TEST_PROC(maths__vector3__dot_of_orthogonal_vectors_is_0)
{
    Vector3 i{ 1, 0, 0 };
    Vector3 j{ 0, 1, 0 };
    Vector3 k{ 0, 0, 1 };

    ASSERT(dot(i, j) == 0);
    ASSERT(dot(j, k) == 0);
    ASSERT(dot(k, i) == 0);
}

TEST_PROC(maths__vector3__cross_of_orthogonal_ijk_is_plus_minus_kji)
{
    Vector3 i{ 1, 0, 0 };
    Vector3 j{ 0, 1, 0 };
    Vector3 k{ 0, 0, 1 };

    ASSERT(cross(i, j) ==  k);
    ASSERT(cross(j, i) == -k);
    ASSERT(cross(j, k) ==  i);
    ASSERT(cross(k, j) == -i);
    ASSERT(cross(k, i) ==  j);
    ASSERT(cross(i, k) == -j);
}

TEST_PROC(maths__vector3__cross_of_zero_vector_is_always_zero)
{
    {
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 1, 2, 3 };
        ASSERT(cross(a, b) == Vector3{ 0, 0, 0 });
        ASSERT(cross(b, a) == Vector3{ 0, 0, 0 });
    }

    {
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 0, 0, 0 };
        ASSERT(cross(a, b) == Vector3{ 0, 0, 0 });
        ASSERT(cross(b, a) == Vector3{ 0, 0, 0 });
    }
}

TEST_PROC(maths__vector3__cross_is_anti_commutative)
{
    { // Basic orthogonal vectors
        Vector3 a{ 1, 0, 0 };
        Vector3 b{ 0, 1, 0 };
        ASSERT(cross(a, b) == -cross(b, a));
        ASSERT(cross(a, b) !=  cross(b, a));
    }

    { // Non-orthogonal vectors
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        ASSERT(cross(a, b) == -cross(b, a));
        ASSERT(cross(a, b) !=  cross(b, a));
    }

    { // Zero vector case is commutative because it is always zero
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 1, 2, 3 };
        ASSERT(cross(a, b) == Vector3{ 0, 0, 0 });
        ASSERT(cross(a, b) == -cross(b, a));
        ASSERT(cross(a, b) ==  cross(b, a));
    }
}

TEST_PROC(maths__vector3__cross_distributive_law)
{
    { // Basic distributive test
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        Vector3 c{ 7, 8, 9 };
        ASSERT(cross(a, b + c) == cross(a, b) + cross(a, c));
    }
    { // With zero vector
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 0, 0, 0 };
        Vector3 c{ 4, 5, 6 };
        ASSERT(cross(a, b + c) == cross(a, b) + cross(a, c));
    }
}

TEST_PROC(maths__vector3__cross_scalar_factorisation)
{
    { // Scalar multiplication
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        float k = 2.0f;
        ASSERT(cross(k * a, b) == k * cross(a, b));
    }
    { // Zero scalar
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        float k = 0.0f;
        ASSERT(cross(k * a, b) == k * cross(a, b));
    }
}

TEST_PROC(maths__vector3__vector_triple_product_abc_is_bac_minus_cab)
{
    { // Basic test case
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        Vector3 c{ 7, 8, 9 };
        ASSERT(cross(a, cross(b, c)) == b*dot(a, c) - c*dot(a, b));
    }

    { // With zero vector
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 4, 5, 6 };
        Vector3 c{ 7, 8, 9 };
        ASSERT(cross(a, cross(b, c)) == b*dot(a, c) - c*dot(a, b));
    }
}

TEST_PROC(maths__vector3__a_cross_b_squared_is_lagranges_identity)
{
    { // Basic test case
        Vector3 a{ 1, 2, 3 };
        Vector3 b{ 4, 5, 6 };
        f32 cross_squared = dot(cross(a, b), cross(a, b));
        f32 expected = dot(a, a) * dot(b, b) - dot(a, b) * dot(a, b);
        ASSERT(almost_equal(cross_squared, expected, 1e-6f));
    }

    { // Perpendicular vectors
        Vector3 a{ 1, 0, 0 };
        Vector3 b{ 0, 1, 0 };
        f32 cross_squared = dot(cross(a, b), cross(a, b));
        f32 expected = dot(a, a) * dot(b, b) - dot(a, b) * dot(a, b);
        ASSERT(almost_equal(cross_squared, expected, 1e-6f));
    }

    { // One zero vector
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 1, 2, 3 };
        f32 cross_squared = dot(cross(a, b), cross(a, b));
        f32 expected = dot(a, a) * dot(b, b) - dot(a, b) * dot(a, b);
        ASSERT(almost_equal(cross_squared, expected, 1e-6f));
    }
}

TEST_PROC(maths__vector3__scalar_triple_product_of_ijk_is_1)
{
    Vector3 i{ 1, 0, 0 };
    Vector3 j{ 0, 1, 0 };
    Vector3 k{ 0, 0, 1 };

    ASSERT(triple_prod(i, j, k) == 1);
    ASSERT(triple_prod(i, j, k) == dot(cross(i, j), k));
    ASSERT(triple_prod(i, j, k) == dot(cross(j, k), i));
    ASSERT(triple_prod(i, j, k) == dot(cross(k, i), j));
}

TEST_PROC(maths__vector3__a_plus_b_satisfies_triangle_inequality)
{
    { // Basic triangle with positive sides
        Vector3 a{ 3, 0, 0 };
        Vector3 b{ 4, 0, 0 };
        ASSERT(length(a) + length(b) >= length(a + b));
    }

    { // Sides that form a degenerate triangle (almost inline)
        Vector3 a{ 1, 0, 0 };
        Vector3 b{ 2, 0, 0 };
        ASSERT(length(a) + length(b) >= length(a + b));
    }

    { // Zero vector case
        Vector3 a{ 0, 0, 0 };
        Vector3 b{ 5, 0, 0 };
        ASSERT(length(a) + length(b) >= length(a + b));
    }

    { // Non-colinear vectors
        Vector3 a{ 3, 4, 0 };
        Vector3 b{ 1, 2, 2 };
        ASSERT(length(a) + length(b) > length(a + b));
    }
}

TEST_PROC(maths__vector3__a_minus_b_satisfies_reverse_triangle_inequality)
{
    { // Basic triangle with positive sides
        Vector3 a{ 3, 0, 0 };
        Vector3 b{ 1, 0, 0 };
        ASSERT(abs(length(a) - length(b)) <= length(a - b));
    }
    { // Almost same length vectors
        Vector3 a{ 5, 0, 0 };
        Vector3 b{ 4, 0, 0 };
        ASSERT(abs(length(a) - length(b)) <= length(a - b));
    }
    { // Zero vector case
        Vector3 a{ 5, 0, 0 };
        Vector3 b{ 0, 0, 0 };
        ASSERT(abs(length(a) - length(b)) <= length(a - b));
    }
    { // Non-colinear vectors
        Vector3 a{ 3, 4, 0 };
        Vector3 b{ 1, 2, 2 };
        ASSERT(abs(length(a) - length(b)) <= length(a - b));
    }
}

TEST_PROC(maths__vector4__operators)
{
    {
        Vector4 a{ 1, 2, 3, 5 };
        Vector4 b{ 7, 9, 11, 13 };
        Vector4 c = a + b;
        ASSERT(c.x == 8);
        ASSERT(c.y == 11);
        ASSERT(c.z == 14);
        ASSERT(c.w == 18);
    }
    {
        Vector4 a{ 1, 2, 3, 5 };
        f32 s = 7;
        Vector4 c = a + s;
        ASSERT(c.x == 8);
        ASSERT(c.y == 9);
        ASSERT(c.z == 10);
        ASSERT(c.w == 12);
    }

    {
        Vector4 a{ 1, 2, 3, 5 };
        Vector4 b{ 7, 9, 11, 13 };
        Vector4 c = a - b;
        ASSERT(c.x == -6);
        ASSERT(c.y == -7);
        ASSERT(c.z == -8);
        ASSERT(c.w == -8);
    }
    {
        Vector4 a{ 1, 2, 3, 5 };
        f32 s = 7;
        Vector4 c = a - s;
        ASSERT(c.x == -6);
        ASSERT(c.y == -5);
        ASSERT(c.z == -4);
        ASSERT(c.w == -2);
    }

    {
        Vector4 a{ 1, 2, 3, 5 };
        f32 s = 7;
        Vector4 c = a * s;
        ASSERT(c.x == 7);
        ASSERT(c.y == 14);
        ASSERT(c.z == 21);
        ASSERT(c.w == 35);
    }

    {
        Vector4 a{ 1, 2, 4, 8 };
        f32 s = 16;
        Vector4 c = a / s;
        ASSERT(c.x == 0.0625f);
        ASSERT(c.y == 0.125f);
        ASSERT(c.z == 0.25f);
        ASSERT(c.w == 0.5f);
    }
}

TEST_PROC(maths__vector4__vec_mul_vec_equals_component_wise_mul)
{
    {
        Vector4 a{ 1, 2, 3, 5 };
        Vector4 b{ 7, 9, 11, 13 };
        Vector4 c = a * b;
        ASSERT(c.x == 7);
        ASSERT(c.y == 18);
        ASSERT(c.z == 33);
        ASSERT(c.w == 65);
    }
}

TEST_PROC(maths__vector4__vec_div_vec_equals_component_wise_div)
{
    {
        Vector4 a{ 1, 2, 4, 8 };
        Vector4 b{ 8, 16, 32, 64 };
        Vector4 c = a / b;
        ASSERT(c.x == 0.125f);
        ASSERT(c.y == 0.125f);
        ASSERT(c.z == 0.125f);
        ASSERT(c.w == 0.125f);
    }
}

TEST_PROC(maths__vector4__swizzle)
{
    {
        Vector4 a{ 1, 2, 3, 5 };
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 5);

        ASSERT(a.x == 1);
        ASSERT(a.y == 2);
        ASSERT(a.z == 3);
        ASSERT(a.w == 5);

        ASSERT(a.r == 1);
        ASSERT(a.g == 2);
        ASSERT(a.b == 3);
        ASSERT(a.a == 5);

        ASSERT(a.xy == Vector2{ 1, 2 });
        ASSERT(a.zw == Vector2{ 3, 5 });
        ASSERT(a.xyz == Vector3{ 1, 2, 3 });

        ASSERT(a.rg == Vector2{ 1, 2 });
        ASSERT(a.ba == Vector2{ 3, 5 });
        ASSERT(a.rgb == Vector3{ 1, 2, 3 });
    }
}

TEST_PROC(maths__mat3__rotate_x)
{
    { // 0 degree rotation
        Matrix3 m = mat3_rotate_x(0.0f);
        ASSERT(m.m00 == 1.0f);
        ASSERT(m.m01 == 0.0f);
        ASSERT(m.m02 == 0.0f);

        ASSERT(m.m10 == 0.0f);
        ASSERT(almost_equal(m.m11, 1.0f));
        ASSERT(almost_equal(m.m12, 0.0f));

        ASSERT(m.m20 == 0.0f);
        ASSERT(almost_equal(m.m21, 0.0f));
        ASSERT(almost_equal(m.m22, 1.0f));
    }

    { // 90 degree rotation
        Matrix3 m = mat3_rotate_x(f32_PI/2);
        ASSERT(m.m00 == 1.0f);
        ASSERT(m.m01 == 0.0f);
        ASSERT(m.m02 == 0.0f);

        ASSERT(m.m10 == 0.0f);
        ASSERT(almost_equal(m.m11,  0.0f));
        ASSERT(almost_equal(m.m12, -1.0f));

        ASSERT(m.m20 == 0.0f);
        ASSERT(almost_equal(m.m21, 1.0f));
        ASSERT(almost_equal(m.m22, 0.0f));
    }

    { // 180 degree rotation
        Matrix3 m = mat3_rotate_x(f32_PI);
        ASSERT(m.m00 == 1.0f);
        ASSERT(m.m01 == 0.0f);
        ASSERT(m.m02 == 0.0f);

        ASSERT(m.m10 == 0.0f);
        ASSERT(almost_equal(m.m11, -1.0f));
        ASSERT(almost_equal(m.m12,  0.0f));

        ASSERT(m.m20 == 0.0f);
        ASSERT(almost_equal(m.m21,  0.0f));
        ASSERT(almost_equal(m.m22, -1.0f));
    }

    { // 360 degree rotation
        Matrix3 m = mat3_rotate_x(2*f32_PI);
        ASSERT(m.m00 == 1.0f);
        ASSERT(m.m01 == 0.0f);
        ASSERT(m.m02 == 0.0f);

        ASSERT(m.m10 == 0.0f);
        ASSERT(almost_equal(m.m11, 1.0f));
        ASSERT(almost_equal(m.m12, 0.0f));

        ASSERT(m.m20 == 0.0f);
        ASSERT(almost_equal(m.m21, 0.0f));
        ASSERT(almost_equal(m.m22, 1.0f));
    }

}

TEST_PROC(maths__mat3__rotate_y)
{
    { // 0 degree rotation
        Matrix3 m = mat3_rotate_y(0.0f);
        ASSERT(almost_equal(m.m00, 1.0f));
        ASSERT(m.m01 == 0.0f);
        ASSERT(almost_equal(m.m02, 0.0f));

        ASSERT(m.m10 == 0.0f);
        ASSERT(m.m11 == 1.0f);
        ASSERT(m.m12 == 0.0f);

        ASSERT(almost_equal(m.m20, 0.0f));
        ASSERT(m.m21 == 0.0f);
        ASSERT(almost_equal(m.m22, 1.0f));
    }

    { // 90 degree rotation
        Matrix3 m = mat3_rotate_y(f32_PI/2);
        ASSERT(almost_equal(m.m00,  0.0f));
        ASSERT(m.m01 == 0.0f);
        ASSERT(almost_equal(m.m02, 1.0f));

        ASSERT(m.m10 == 0.0f);
        ASSERT(m.m11 == 1.0f);
        ASSERT(m.m12 == 0.0f);

        ASSERT(almost_equal(m.m20, -1.0f));
        ASSERT(m.m21 == 0.0f);
        ASSERT(almost_equal(m.m22, 0.0f));
    }

    { // 180 degree rotation
        Matrix3 m = mat3_rotate_y(f32_PI);
        ASSERT(almost_equal(m.m00, -1.0f));
        ASSERT(m.m01 == 0.0f);
        ASSERT(almost_equal(m.m02,  0.0f));

        ASSERT(m.m10 == 0.0f);
        ASSERT(m.m11 == 1.0f);
        ASSERT(m.m12 == 0.0f);

        ASSERT(almost_equal(m.m20, 0.0f));
        ASSERT(m.m21 == 0.0f);
        ASSERT(almost_equal(m.m22, -1.0f));
    }
}

TEST_PROC(maths__mat3__rotate_z)
{
    { // 0 degree rotation
        Matrix3 m = mat3_rotate_z(0.0f);
        ASSERT(m.m00 == 1.0f);
        ASSERT(m.m01 == 0.0f);
        ASSERT(m.m02 == 0.0f);

        ASSERT(m.m10 == 0.0f);
        ASSERT(m.m11 == 1.0f);
        ASSERT(m.m12 == 0.0f);

        ASSERT(m.m20 == 0.0f);
        ASSERT(m.m21 == 0.0f);
        ASSERT(m.m22 == 1.0f);
    }

    { // 90 degree rotation
        Matrix3 m = mat3_rotate_z(f32_PI/2);
        ASSERT(almost_equal(m.m00,  0.0f));
        ASSERT(almost_equal(m.m01, -1.0f));
        ASSERT(m.m02 == 0.0f);

        ASSERT(almost_equal(m.m10,  1.0f));
        ASSERT(almost_equal(m.m11,  0.0f));
        ASSERT(m.m12 == 0.0f);

        ASSERT(m.m20 == 0.0f);
        ASSERT(m.m21 == 0.0f);
        ASSERT(m.m22 == 1.0f);
    }

    { // 180 degree rotation
        Matrix3 m = mat3_rotate_z(f32_PI);
        ASSERT(almost_equal(m.m00, -1.0f));
        ASSERT(almost_equal(m.m01,  0.0f));
        ASSERT(m.m02 == 0.0f);

        ASSERT(almost_equal(m.m10,  0.0f));
        ASSERT(almost_equal(m.m11, -1.0f));
        ASSERT(m.m12 == 0.0f);

        ASSERT(m.m20 == 0.0f);
        ASSERT(m.m21 == 0.0f);
        ASSERT(m.m22 == 1.0f);
    }
}

TEST_PROC(maths__mat3__rotate_axis)
{
    { // rotation around x-axis (same as mat3_rotate_x)
        Vector3 x_axis = {1.0f, 0.0f, 0.0f};
        Matrix3 m = mat3_rotate(x_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate_x(f32_PI/2)));

        ASSERT(almost_equal(m.m00, 1.0f));
        ASSERT(almost_equal(m.m01, 0.0f));
        ASSERT(almost_equal(m.m02, 0.0f));

        ASSERT(almost_equal(m.m10,  0.0f));
        ASSERT(almost_equal(m.m11,  0.0f));
        ASSERT(almost_equal(m.m12, -1.0f));

        ASSERT(almost_equal(m.m20, 0.0f));
        ASSERT(almost_equal(m.m21, 1.0f));
        ASSERT(almost_equal(m.m22, 0.0f));
    }

    { // rotation around y-axis (same as mat3_rotate_y)
        Vector3 y_axis = {0.0f, 1.0f, 0.0f};
        Matrix3 m = mat3_rotate(y_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate_y(f32_PI/2)));

        ASSERT(almost_equal(m.m00, 0.0f));
        ASSERT(almost_equal(m.m01, 0.0f));
        ASSERT(almost_equal(m.m02, 1.0f));

        ASSERT(almost_equal(m.m10, 0.0f));
        ASSERT(almost_equal(m.m11, 1.0f));
        ASSERT(almost_equal(m.m12, 0.0f));

        ASSERT(almost_equal(m.m20, -1.0f));
        ASSERT(almost_equal(m.m21,  0.0f));
        ASSERT(almost_equal(m.m22,  0.0f));
    }

    { // rotation around z-axis (same as mat3_rotate_z)
        Vector3 z_axis = {0.0f, 0.0f, 1.0f};
        Matrix3 m = mat3_rotate(z_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate_z(f32_PI/2)));

        ASSERT(almost_equal(m.m00,  0.0f));
        ASSERT(almost_equal(m.m01, -1.0f));
        ASSERT(almost_equal(m.m02,  0.0f));

        ASSERT(almost_equal(m.m10, 1.0f));
        ASSERT(almost_equal(m.m11, 0.0f));
        ASSERT(almost_equal(m.m12, 0.0f));

        ASSERT(almost_equal(m.m20, 0.0f));
        ASSERT(almost_equal(m.m21, 0.0f));
        ASSERT(almost_equal(m.m22, 1.0f));
    }
}

TEST_PROC(maths__mat3__determinant_of_identity_is_1)
{
    Matrix3 M = mat3_identity();
    ASSERT(almost_equal(mat3_determinant(M), 1, 0.0001f));
}


TEST_PROC(maths__mat3__determinant_of_M_equals_determinant_of_M_transposed)
{
    {
        Matrix3 M = mat3_identity();
        Matrix3 Mt = mat3_transpose(M);
        ASSERT(almost_equal(mat3_determinant(M), mat3_determinant(Mt), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic(-1, 1, 1, -1);
        Matrix3 Mt = mat3_transpose(M);
        ASSERT(almost_equal(mat3_determinant(M), mat3_determinant(Mt), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic(-5, 5, 3, -2);
        Matrix3 Mt = mat3_transpose(M);
        ASSERT(almost_equal(mat3_determinant(M), mat3_determinant(Mt), 0.0001f));
    }
}

TEST_PROC(maths__mat3__M_times_M_inverse_equals_identity)
{
    {
        Matrix3 M = mat3_identity();
        Matrix3 Mi = mat3_inverse(M);
        ASSERT(almost_equal(M*Mi, mat3_identity(), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic(-1, 1, 1, -1);
        Matrix3 Mi = mat3_inverse(M);
        ASSERT(almost_equal(M*Mi, mat3_identity(), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic(-5, 5, 3, -2);
        Matrix3 Mi = mat3_inverse(M);
        ASSERT(almost_equal(M*Mi, mat3_identity(), 0.0001f));
    }

}

TEST_PROC(maths__mat4__M_times_M_inverse_equals_identity)
{
    {
        Matrix4 M = mat4_identity();
        Matrix4 Mi = mat4_inverse(M);
        ASSERT(almost_equal(M*Mi, mat4_identity(), 0.0001f));
    }

    {
        Matrix4 M = mat4_orthographic(-5, 5, 3, -2, 0.0001f, 100.0f);
        Matrix4 Mi = mat4_inverse(M);
        ASSERT(almost_equal(M*Mi, mat4_identity(), 0.0001f));
    }

}

TEST_PROC(maths__quaternion__constructors)
{
    {
        Quaternion q{};
        ASSERT(q.x == 0);
        ASSERT(q.y == 0);
        ASSERT(q.z == 0);
        ASSERT(q.w == 0);
    }

    {
        Quaternion q = quat_identity();
        ASSERT(q.x == 0);
        ASSERT(q.y == 0);
        ASSERT(q.z == 0);
        ASSERT(q.w == 1);
    }

    {
        Quaternion q{ 1, 2, 3, 4 };
        ASSERT(q.x == 1);
        ASSERT(q.y == 2);
        ASSERT(q.z == 3);
        ASSERT(q.w == 4);
    }

    {
        Quaternion q{ .xyzw.xyz = Vector3{ 1, 2, 3 }, 4 };
        ASSERT(q.x == 1);
        ASSERT(q.y == 2);
        ASSERT(q.z == 3);
        ASSERT(q.w == 4);
    }
}

TEST_PROC(maths__quaternion__swizzle)
{
    {
        Quaternion q{ 1, 2, 3, 4 };
        ASSERT(q[0] == 1);
        ASSERT(q[1] == 2);
        ASSERT(q[2] == 3);
        ASSERT(q[3] == 4);

        ASSERT(q.x == 1);
        ASSERT(q.y == 2);
        ASSERT(q.z == 3);
        ASSERT(q.w == 4);

        ASSERT(q.xyzw == Vector4{ 1, 2, 3, 4 });
    }
}

TEST_PROC(maths__quaternion__angle_axis)
{
    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        ASSERT(q.x == 0);
        ASSERT(q.y == 0);
        ASSERT(q.z == 0.707106769f);
        ASSERT(q.w == 0.707106769f);
    }

    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });
        ASSERT(q.x == 0);
        ASSERT(q.y == 0.707106769f);
        ASSERT(q.z == 0);
        ASSERT(q.w == 0.707106769f);
    }

    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
        ASSERT(q.x == 0.707106769f);
        ASSERT(q.y == 0);
        ASSERT(q.z == 0);
        ASSERT(q.w == 0.707106769f);
    }
}

TEST_PROC(maths__quaternion__euler)
{
    {
        Quaternion q = quat_euler(0, 0, 0);
        ASSERT(q.x == 0);
        ASSERT(q.y == 0);
        ASSERT(q.z == 0);
        ASSERT(q.w == 1);

        Vector3 v = q*Vector3{ 1, 2, 3 };
        ASSERT(v.x == 1);
        ASSERT(v.y == 2);
        ASSERT(v.z == 3);
    }

    {
        Quaternion q = quat_euler(f32_PI/2.0f, 0, 0);
        ASSERT(almost_equal(q.x, 0.707106769f, 0.0001f));
        ASSERT(q.y == 0);
        ASSERT(q.z == 0);
        ASSERT(almost_equal(q.w, 0.707106769f, 0.0001f));

        Vector3 v = q*Vector3{ 1, 2, 3 };
        ASSERT(almost_equal(v.x, 1, 0.0001f));
        ASSERT(almost_equal(v.y, -3, 0.0001f));
        ASSERT(almost_equal(v.z, 2, 0.0001f));
    }

    {
        Quaternion q = quat_euler(0, f32_PI/2.0f, 0);
        ASSERT(q.x == 0);
        ASSERT(almost_equal(q.y, 0.707106769f, 0.0001f));
        ASSERT(q.z == 0);
        ASSERT(almost_equal(q.w, 0.707106769f, 0.0001f));

        Vector3 v = q*Vector3{ 1, 2, 3 };
        ASSERT(almost_equal(v.x, 3, 0.0001f));
        ASSERT(almost_equal(v.y, 2, 0.0001f));
        ASSERT(almost_equal(v.z, -1, 0.0001f));
    }

    {
        Quaternion q = quat_euler(0, 0, f32_PI/2.0f);
        ASSERT(q.x == 0);
        ASSERT(q.y == 0);
        ASSERT(almost_equal(q.z, 0.707106769f, 0.0001f));
        ASSERT(almost_equal(q.w, 0.707106769f, 0.0001f));

        Vector3 v = q*Vector3{ 1, 2, 3 };
        ASSERT(almost_equal(v.x, -2, 0.0001f));
        ASSERT(almost_equal(v.y, 1, 0.0001f));
        ASSERT(almost_equal(v.z, 3, 0.0001f));
    }
}

TEST_PROC(maths__quaternion__inverse)
{
    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        Quaternion inv = quat_inverse(q);
        ASSERT(inv.x == 0);
        ASSERT(inv.y == 0);
        ASSERT(almost_equal(inv.z, -0.707106769f, 0.0001f));
        ASSERT(almost_equal(inv.w, 0.707106769f, 0.0001f));

        Vector3 v{ 1, 2, 3 };
        Vector3 r = q*v*inv;
        ASSERT(almost_equal(r.x, v.x, 0.0001f));
        ASSERT(almost_equal(r.y, v.y, 0.0001f));
        ASSERT(almost_equal(r.z, v.z, 0.0001f));
    }

    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });
        Quaternion inv = quat_inverse(q);
        ASSERT(inv.x == 0);
        ASSERT(almost_equal(inv.y, -0.707106769f, 0.0001f));
        ASSERT(inv.z == 0);
        ASSERT(almost_equal(inv.w, 0.707106769f, 0.0001f));

        Vector3 v{ 1, 2, 3 };
        Vector3 r = q*v*inv;
        ASSERT(almost_equal(r.x, v.x, 0.0001f));
        ASSERT(almost_equal(r.y, v.y, 0.0001f));
        ASSERT(almost_equal(r.z, v.z, 0.0001f));
    }

    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
        Quaternion inv = quat_inverse(q);
        ASSERT(almost_equal(inv.x, -0.707106769f, 0.0001f));
        ASSERT(inv.y == 0);
        ASSERT(inv.z == 0);
        ASSERT(almost_equal(inv.w, 0.707106769f, 0.0001f));

        Vector3 v{ 1, 2, 3 };
        Vector3 r = q*v*inv;
        ASSERT(almost_equal(r.x, v.x, 0.0001f));
        ASSERT(almost_equal(r.y, v.y, 0.0001f));
        ASSERT(almost_equal(r.z, v.z, 0.0001f));
    }
}

TEST_PROC(maths__quaternion__add_is_componentwise)
{
    Quaternion p{ 1, 2, 3, 4 };
    Quaternion q{ 5, 6, 7, 8 };
    ASSERT(p+q == Quaternion{ 6, 8, 10, 12 });
}

TEST_PROC(maths__quaternion__add_is_associative)
{
    Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
    Quaternion q3 = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });

    ASSERT((q1 + q2) + q3 == q1 + (q2 + q3));
}

TEST_PROC(maths__quaternion__add_is_commutative)
{
    Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });

    ASSERT(q1 + q2 == q2 + q1);
}

TEST_PROC(maths__quaternion__scalar_mul_quat_is_distributive)
{
    {
        Quaternion p = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        f32 t = 2;
        ASSERT(t*(p+q) == t*p + t*q);
    }

    {
        Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        f32 s = 2, t = 3;
        ASSERT((s+t)*q == s*q + t*q);
    }
}

TEST_PROC(maths__quaternion__mul_is_associative)
{
    Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
    Quaternion q3 = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });

    ASSERT(q1*q2*q3 == (q1*q2)*q3);
    ASSERT(q1*q2*q3 == q1*(q2*q3));
}

TEST_PROC(maths__quaternion__mul_is_distributive)
{
    Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 1, 0, 0 });
    Quaternion q3 = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });
    ASSERT(q1*(q2+q3) == q1*q2 + q1*q3);
    ASSERT((q1+q2)*q3 == q1*q3 + q2*q3);
}

TEST_PROC(maths__quaternion__q2_mul_q1_equals_q1q2_minus_2v1_cross_v2)
{
    {
        Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 0, 1, 0 });
        ASSERT(almost_equal(q2*q1, q1*q2 - 2*cross(q1.xyz, q2.xyz)));
    }

    { // when the vector parts are parallel, the quaternion multiplication is commutative
        Quaternion q1 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        Quaternion q2 = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
        ASSERT(almost_equal(q2*q1, q1*q2 - 2*cross(q1.xyz, q2.xyz)));
        ASSERT(almost_equal(q2*q1, q1*q2));
    }
}
TEST_PROC(maths__quaternion__q_mul_q_conj_is_commutative)
{
    Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion r1 = quat_conjugate(q)*q;
    Quaternion r2 = q*quat_conjugate(q);
    ASSERT(almost_equal(r1, r2));
}

TEST_PROC(maths__quaternion__q_mul_q_conj_equals_v_sq_plus_s_sq)
{
    Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    Quaternion qs = quat_conjugate(q);
    Quaternion r = q*qs;
    ASSERT(almost_equal(r.w, length_sq(q)));
    ASSERT(almost_equal(r.w, dot(q.v, q.v) + q.s*q.s));
    ASSERT(almost_equal(r.x, 0));
    ASSERT(almost_equal(r.y, 0));
    ASSERT(almost_equal(r.z, 0));
}

TEST_PROC(maths__misc__angle_between)
{
    {
        Vector3 up = { 0, 1, 0 };
        Vector3 right = { 1, 0, 0 };
        f32 angle = angle_between(up, right);
        ASSERT(angle == f32_PI/2.0f);
    }

    {
        Vector3 up = { 0, 1, 0 };
        Vector3 left = { -1, 0, 0 };
        f32 angle = angle_between(up, left);
        ASSERT(angle == f32_PI/2.0f);
    }

    {
        Vector3 up{ 0, 1, 0 };
        Vector3 forward{ 0, 0, 1 };
        f32 angle = angle_between(up, forward);
        ASSERT(angle == f32_PI/2.0f);
    }
}

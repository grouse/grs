#include "maths.h"
#include "core.h"
#include "array.h"

// -- Vector2
Vector2 abs(Vector2 v) EXPORT
{
    // TODO(jesper): cmath dependency
    return Vector2{ fabsf(v.x), fabsf(v.y) };
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

Vector3 reflect(Vector3 v, Vector3 n) EXPORT { return v - 2.0f*dot(v, n)*n; }

Vector3 refract(Vector3 v, Vector3 n, f32 etai_over_etat) EXPORT
{
    f32 cos_theta = MIN(dot(-v, n), 1.0f);
    Vector3 d_perp = etai_over_etat * (v + cos_theta*n);
    Vector3 d_parallel = -sqrtf(fabs(1.0f-dot(d_perp, d_perp))) * n;
    return d_perp + d_parallel;
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

Vector3 operator+(f32 scalar, Vector3 v)
{
    Vector3 r;
    r.x = scalar + v.x;
    r.y = scalar + v.y;
    r.z = scalar + v.z;
    return r;
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

Vector3 operator*(f32 scalar, Vector3 v)
{
    Vector3 r;
    r.x = scalar * v.x;
    r.y = scalar * v.y;
    r.z = scalar * v.z;
    return r;
}

Vector3 operator*(Vector3 v, f32 scalar)
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

Vector3 operator/(Vector3 v, f32 scalar)
{
    Vector3 r;
    r.x = v.x/scalar;
    r.y = v.y/scalar;
    r.z = v.z/scalar;
    return r;
}

f32 dot(Vector3 lhs, Vector3 rhs) EXPORT { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

Vector3 cross(Vector3 lhs, Vector3 rhs) EXPORT
{
    Vector3 r;
    r.x = lhs.y * rhs.z - lhs.z * rhs.y;
    r.y = lhs.z * rhs.x - lhs.x * rhs.z;
    r.z = lhs.x * rhs.y - lhs.y * rhs.x;
    return r;
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

Vector4 operator*(Vector4 v, f32 scalar)
{
    Vector4 r;
    r.x = v.x*scalar;
    r.y = v.y*scalar;
    r.z = v.z*scalar;
    r.w = v.w*scalar;
    return r;
}

Vector4 operator*(f32 scalar, Vector4 v)
{
    Vector4 r;
    r.x = v.x*scalar;
    r.y = v.y*scalar;
    r.z = v.z*scalar;
    r.w = v.w*scalar;
    return r;
}

Vector4 operator*(Vector4 lhs, Vector4 rhs)
{
    Vector4 r;
    r.x = lhs.x * rhs.x;
    r.y = lhs.y * rhs.y;
    r.z = lhs.z * rhs.z;
    r.w = lhs.w * rhs.w;
    return r;
}

Vector4 operator+(Vector4 lhs, Vector4 rhs)
{
    Vector4 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    r.z = lhs.z + rhs.z;
    r.w = lhs.w + rhs.w;
    return r;
}

Vector4 operator-(Vector4 lhs, Vector4 rhs)
{
    Vector4 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    r.z = lhs.z - rhs.z;
    r.w = lhs.w - rhs.w;
    return r;
}

Vector4 operator*(Matrix4 m, Vector4 v)
{
    Vector4 r;
    r[0] = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]*v.w;
    r[1] = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]*v.w;
    r[2] = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]*v.w;
    r[3] = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]*v.w;
    return r;
}

Vector4 operator/(Vector4 v, f32 scalar)
{
    Vector4 r;
    r.x = v.x/scalar;
    r.y = v.y/scalar;
    r.z = v.z/scalar;
    r.w = v.w/scalar;
    return r;
}

f32 dot(Vector4 lhs, Vector4 rhs) EXPORT
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}


// Quaternion
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

Quaternion quat_identity() EXPORT { return { 0, 0, 0, 1 }; }

Quaternion quat_rotate(Vector3 v, f32 theta) EXPORT
{
    f32 half_theta = theta/2;
    f32 s = sinf(half_theta);
    return { v.x*s, v.y*s, v.z*s, cosf(half_theta) };
}
Quaternion quat_yaw(f32 theta) EXPORT { return quat_rotate({ 0, 1, 0 }, theta); }
Quaternion quat_pitch(f32 theta) EXPORT { return quat_rotate({ 1, 0, 0 }, theta); }
Quaternion quat_roll(f32 theta) EXPORT { return quat_rotate({ 0, 0, 1 }, theta); }

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

Quaternion operator*(Quaternion p, Quaternion q) EXPORT
{
    Quaternion r;
    r.x = p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
    r.y = p.w*q.y - p.x*q.z + p.y*q.w + p.z*q.x;
    r.z = p.w*q.z + p.x*q.y - p.y*q.x + p.z*q.w;
    r.w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;
    return r;
}

Vector3 operator*(Quaternion q, Vector3 v) EXPORT
{
    Vector3 t = 2.0f * cross(q.xyz, v);
    return v + q.w*t + cross(q.xyz, t);
}



// Matrix3
Matrix3 mat3_identity() EXPORT
{
    Matrix3 r{};
    r.columns[0].data[0] = 1.0f;
    r.columns[1].data[1] = 1.0f;
    r.columns[2].data[2] = 1.0f;
    return r;
}

Matrix3 orthographic3(f32 left, f32 right, f32 bottom, f32 top, f32 ratio /*=1*/) EXPORT
{
    Matrix3 r = mat3_identity();

    r[0][0] = 2 / (right-left);
    r[1][1] = (2 / (top-bottom)) * ratio;
    r[2][0] = -(right+left) / (right-left);
    r[2][1] = (-(top+bottom) / (top-bottom)) * ratio;

    return r;
}

Matrix3 translate(Matrix3 m, Vector3 v) EXPORT
{
    Matrix3 r = m;
    r[2] = m[0]*v[0] + m[1]*v[1] + m[2]*v[2];
    return r;
}

Matrix3 scale(Matrix3 m, f32 scalar) EXPORT
{
    Matrix3 r;
    r[0] = m[0] * scalar;
    r[1] = m[1] * scalar;
    r[2] = m[2] * scalar;
    return r;
}

Matrix3 transform(Matrix3 projection, Vector2 position) EXPORT
{
    Matrix3 view = translate(mat3_identity(), { .xy = -position, 1 });
    return projection*view;
}

Matrix3 transform(Matrix3 projection, Vector2 position, f32 uni_scale) EXPORT
{
    Matrix3 mview = translate(mat3_identity(), { .xy = -position, 1 });
    Matrix3 mscale = scale(mat3_identity(), uni_scale);
    return projection*mview*mscale;
}

Matrix3 transpose(Matrix3 m) EXPORT
{
    Matrix3 r;
    r[0][0] = m[0][0];
    r[0][1] = m[1][0];
    r[0][2] = m[2][0];
    r[0][3] = m[3][0];

    r[1][0] = m[0][1];
    r[1][1] = m[1][1];
    r[1][2] = m[2][1];
    r[1][3] = m[3][1];

    r[2][0] = m[0][2];
    r[2][1] = m[1][2];
    r[2][2] = m[2][2];
    r[2][3] = m[3][2];

    r[3][0] = m[0][3];
    r[3][1] = m[1][3];
    r[3][2] = m[2][3];
    r[3][3] = m[3][3];
    return r;
}

Matrix3 inverse(Matrix3 m) EXPORT
{
    f32 det =
        m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
        m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
        m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);

    f32 inv_det = 1/det;

    Matrix3 r;
    r[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * inv_det;
    r[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]) * inv_det;
    r[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * inv_det;
    r[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]) * inv_det;
    r[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * inv_det;
    r[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * inv_det;
    r[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * inv_det;
    r[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]) * inv_det;
    r[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * inv_det;

    return r;
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

Matrix4 orthographic4(f32 left, f32 right, f32 bottom, f32 top, f32 ratio /*=1*/) EXPORT
{
    Matrix4 r = mat4_identity();

    r[0][0] = 2 / (right-left);
    r[1][1] = (2 / (top-bottom)) * ratio;
    r[3][0] = -(right+left) / (right-left);
    r[3][1] = (-(top+bottom) / (top-bottom)) * ratio;

    return r;
}

Matrix4 mat4_perspective(f32 fov, f32 aspect, f32 near_z, f32 far_z) EXPORT
{
    Matrix4 r{};

    f32 tan_half_fov = tanf(fov/2);
    r[0][0] = 1 / (aspect * tan_half_fov);
    r[1][1] = 1 / tan_half_fov;
    r[2][2] = far_z/(near_z - far_z);
    r[2][3] = -1;
    r[3][2] = -(far_z * near_z)/(far_z - near_z);

    return r;
}

Matrix4 mat4_inv_transform(Vector3 eye, Vector3 forward, Vector3 up) EXPORT
{
    Vector3 Z = normalise(-forward);
    Vector3 X = normalise(cross(up, Z));
    Vector3 Y = cross(Z, X);

    Matrix4 r;
    r[0] = { X[0], Y[0], Z[0], 0 };
    r[1] = { X[1], Y[1], Z[1], 0 };
    r[2] = { X[2], Y[2], Z[2], 0 };
    r[3] = { -dot(X, eye), -dot(Y, eye), -dot(Z, eye), 1 };
    return r;
}

Matrix4 operator*(Matrix4 lhs, Matrix4 rhs) EXPORT
{
    Matrix4 m{};
    m[0] = lhs[0]*rhs[0][0] + lhs[1]*rhs[0][1] + lhs[2]*rhs[0][2] + lhs[3]*rhs[0][3];
    m[1] = lhs[0]*rhs[1][0] + lhs[1]*rhs[1][1] + lhs[2]*rhs[1][2] + lhs[3]*rhs[1][3];
    m[2] = lhs[0]*rhs[2][0] + lhs[1]*rhs[2][1] + lhs[2]*rhs[2][2] + lhs[3]*rhs[2][3];
    m[3] = lhs[0]*rhs[3][0] + lhs[1]*rhs[3][1] + lhs[2]*rhs[3][2] + lhs[3]*rhs[3][3];
    return m;
}


// intersection tests
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
    StaticArray<Line2, 4> aabb_lines;
    calc_aabb_lines(aabb_lines.storage, pos_a, extents_a);

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
    StaticArray<Line2, 4> aabb_lines;
    calc_aabb_lines(aabb_lines.storage, pos, extents);

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

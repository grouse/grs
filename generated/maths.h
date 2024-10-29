#ifndef MATHS_PUBLIC_H
#define MATHS_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern Vector2 abs(Vector2 v);
extern Vector2 vec2_max(Vector2 lhs, Vector2 rhs);
extern Vector2 calc_center(Vector2 tl, Vector2 br, Vector2 size);
extern Vector2 lerp(Vector2 a, Vector2 b, f32 t);
extern f32 length(Vector2 v);
extern f32 length_sq(Vector2 v);
extern Vector2 normalise(Vector2 v);
extern Vector2 tangent(Vector2 v);
extern Vector2 line_normal(Vector2 start, Vector2 end);
extern f32 dot(Vector2 lhs, Vector2 rhs);
extern Vector3 abs(Vector3 v);
extern f32 length(Vector3 v);
extern f32 length_sq(Vector3 v);
extern Vector3 lerp(Vector3 a, Vector3 b, f32 t);
extern Vector3 normalise(Vector3 v);
extern Vector3 reflect(Vector3 v, Vector3 n);
extern Vector3 refract(Vector3 v, Vector3 n, f32 etai_over_etat);
extern f32 dot(Vector3 lhs, Vector3 rhs);
extern Vector3 cross(Vector3 lhs, Vector3 rhs);
extern f32 length(Vector4 v);
extern f32 length_sq(Vector4 v);
extern Vector4 lerp(Vector4 a, Vector4 b, f32 t);
extern Vector4 normalise(Vector4 v);
extern f32 dot(Vector4 lhs, Vector4 rhs);
extern Quaternion quat_identity();
extern Quaternion quat_angle_axis(f32 theta, Vector3 v);
extern Quaternion quat_euler(f32 pitch, f32 yaw, f32 roll);
extern Quaternion quat_euler(Vector3 euler);
extern Quaternion quat_yaw(f32 theta);
extern Quaternion quat_pitch(f32 theta);
extern Quaternion quat_roll(f32 theta);
extern Quaternion quat_from_mat4(Matrix4 trs);
extern Quaternion normalise(Quaternion q);
extern Quaternion quat_conjugate(Quaternion q);
extern Quaternion quat_inverse(Quaternion q);
extern Quaternion operator*(Quaternion p, Quaternion q);
extern Vector3 operator*(Quaternion q, Vector3 v);
extern Vector3 operator*(Vector3 v, Quaternion q);
extern Quaternion operator/(Quaternion q, f32 scalar);
extern Matrix3 mat3_identity();
extern Matrix3 mat3_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 ratio =1);
extern Matrix3 mat3_translate(Matrix3 m, Vector3 v);
extern Matrix3 mat3_scale(Matrix3 m, f32 scalar);
extern Matrix3 mat3_transform(Matrix3 projection, Vector2 position);
extern Matrix3 mat3_transform(Matrix3 projection, Vector2 position, f32 uni_scale);
extern Matrix3 mat3_transpose(Matrix3 m);
extern Matrix3 mat3_inverse(Matrix3 m);
extern Matrix4 mat4_identity();
extern Matrix4 mat4_rotate(Quaternion q);
extern Matrix4 mat4_translate(Vector3 v);
extern Matrix4 mat4_scale(Vector3 v);
extern Matrix4 mat4_transform(Quaternion rotation, Vector3 position);
extern Matrix4 mat4_transform(Quaternion rotation, Vector3 position, Vector3 scale);
extern void mat4_trs_decompose(Matrix4 trs, Vector3 *pos, Quaternion *rot, Vector3 *scale);
extern Matrix4 mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 ratio =1);
extern Matrix4 mat4_perspective(f32 fov, f32 aspect, f32 near_z, f32 far_z);
extern Matrix4 mat4_inv_transform(Vector3 eye, Vector3 forward, Vector3 up);
extern Matrix4 mat4_transpose(Matrix4 m);
extern Matrix4 mat4_inverse(Matrix4 m);
extern Matrix4 operator*(Matrix4 lhs, Matrix4 rhs);
extern Matrix4 operator*(Matrix4 m, f32 scalar);
extern bool ray_intersect_capsule(Vector3 ray_o, Vector3 ray_d, Vector3 cap_p0, Vector3 cap_p1, f32 cap_r, f32 *tr);
extern bool ray_intersect_triangle(Vector3 ray_o, Vector3 ray_d, Vector3 p0, Vector3 p1, Vector3 p2, f32 *tr);
extern bool ray_intersect_quad(Vector3 ray_o, Vector3 ray_d, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, f32 *tr);
extern bool ray_intersect_plane(Vector3 ray_o, Vector3 ray_d, Vector3 plane_n, f32 plane_d, f32 *tr);
extern bool nearest_ray_vs_line(Vector3 ray_o, Vector3 ray_d, Vector3 line_o, Vector3 line_d, f32 *t, f32 *u);
extern bool point_in_aabb(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size, f32 epsilon);
extern bool point_in_circle(Vector2 p, Vector2 c, f32 radius_sq);
extern Vector2 point_clamp_aabb(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size);
extern Vector2 point_clamp_aabb_circle(Vector2 p, Vector2 aabb_pos, Vector2 aabb_half_size, Vector2 c_p, f32 c_r);
extern bool point_in_triangle(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2);
extern bool point_in_rect(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);
extern bool point_in_rect(Vector2 p, Vector2 center, Vector2 size);
extern bool point_in_rect(Vector2 p, Rect r);
extern Contact aabb_intersect_aabb(Vector2 pos_a, Vector2 extents_a, Vector2 pos_b, Vector2 extents_b);
extern Contact aabb_intersect_line(Vector2 pos_a, Vector2 extents_a, Vector2 l_p0, Vector2 l_p1);
extern bool line_intersect_line(Line2 l0, Line2 l1, Vector2 *intersect_point);
extern bool line_intersect_line(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 *intersect_point);
extern bool line_intersect_aabb(Vector2 l_p0, Vector2 l_p1, Vector2 pos, Vector2 half_size, Vector2 *out_intersect_point, Vector2 *out_normal);
extern void calc_aabb_lines(Line2 lines[4], Vector2 pos, Vector2 extents);
extern bool aabb_intersect_circle(Vector2 aabb_p, Vector2 aabb_half_size, Vector2 circle_p, f32 circle_r);
extern bool aabb_intersect_aabb(Vector2 pos_a, Vector2 extents_a, Vector2 pos_b, Vector2 extents_b, Vector2 *out_delta, Vector2 *out_normal);
extern bool aabb_intersect_line(Vector2 pos, Vector2 extents, Vector2 l_p0, Vector2 l_p1, Vector2 *out_intersect_point, Vector2 *out_normal);
extern bool swept_aabb_intersect_line(Vector2 pos, Vector2 half_size, Vector2 delta, Vector2 l_p0, Vector2 l_p1, Vector2 *out_intersect_point, Vector2 *out_normal);
extern bool aabb_intersect_swept_aabb(Vector2 pos_a, Vector2 half_size_a, Vector2 pos_b, Vector2 half_size_b, Vector2 delta_b, Vector2 *out_intersect_point, Vector2 *out_normal);
extern f32 calc_center(f32 min, f32 max, f32 size);
extern f32 round_to(f32 value, f32 multiple);
extern Vector2 round_to(Vector2 v, f32 multiple);
extern f32 lerp(f32 a, f32 b, f32 t);
extern u32 rand_u32(XORShift128 *series);
extern f32 rand_f32(XORShift128 *series);
extern f32 rand_f32(XORShift128 *series, f32 min, f32 max);
extern Vector3 rand_sphere(XORShift128 *series);
extern Vector3 rand_color3(XORShift128 *series);
extern Vector3 rand_hemisphere(Vector3 normal, XORShift128 *series);
extern Vector3 rand_disc(XORShift128 *series);
extern f32 radf(f32 theta);
extern f32 degf(f32 rad);
extern f32 almost_equal(f32 a, f32 b, f32 epsilon);
extern f32 angle_between(Vector3 v0, Vector3 v1);
extern Vector3 rgb_unpack(u32 argb);
extern Vector4 argb_unpack(u32 argb);
extern f32 linear_from_sRGB(f32 s);
extern Vector3 linear_from_sRGB(Vector3 sRGB);
extern Vector4 linear_from_sRGB(Vector4 sRGB);

#endif // MATHS_PUBLIC_H

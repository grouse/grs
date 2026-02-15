#include "core/maths.h"
#include "core/test.h"

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
        ASSERT(fabsf(length(a) - length(b)) <= length(a - b));
    }
    { // Almost same length vectors
        Vector3 a{ 5, 0, 0 };
        Vector3 b{ 4, 0, 0 };
        ASSERT(fabsf(length(a) - length(b)) <= length(a - b));
    }
    { // Zero vector case
        Vector3 a{ 5, 0, 0 };
        Vector3 b{ 0, 0, 0 };
        ASSERT(fabsf(length(a) - length(b)) <= length(a - b));
    }
    { // Non-colinear vectors
        Vector3 a{ 3, 4, 0 };
        Vector3 b{ 1, 2, 2 };
        ASSERT(fabsf(length(a) - length(b)) <= length(a - b));
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
        Matrix3 m = mat3_rotate3_x(0.0f);
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
        Matrix3 m = mat3_rotate3_x(f32_PI/2);
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
        Matrix3 m = mat3_rotate3_x(f32_PI);
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
        Matrix3 m = mat3_rotate3_x(2*f32_PI);
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
        Matrix3 m = mat3_rotate3_y(0.0f);
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
        Matrix3 m = mat3_rotate3_y(f32_PI/2);
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
        Matrix3 m = mat3_rotate3_y(f32_PI);
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
        Matrix3 m = mat3_rotate3_z(0.0f);
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
        Matrix3 m = mat3_rotate3_z(f32_PI/2);
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
        Matrix3 m = mat3_rotate3_z(f32_PI);
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
        Matrix3 m = mat3_rotate3(x_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate3_x(f32_PI/2)));

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
        Matrix3 m = mat3_rotate3(y_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate3_y(f32_PI/2)));

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
        Matrix3 m = mat3_rotate3(z_axis, f32_PI/2);
        ASSERT(almost_equal(m, mat3_rotate3_z(f32_PI/2)));

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
        Matrix3 M = mat3_orthographic2(-1, 1, -1, 1);
        Matrix3 Mt = mat3_transpose(M);
        ASSERT(almost_equal(mat3_determinant(M), mat3_determinant(Mt), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic2(-5, 5, -2, 3);
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
        Matrix3 M = mat3_orthographic2(-1, 1, -1, 1);
        Matrix3 Mi = mat3_inverse(M);
        ASSERT(almost_equal(M*Mi, mat3_identity(), 0.0001f));
    }

    {
        Matrix3 M = mat3_orthographic2(-5, 5, -2, 3);
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
        Matrix4 M = mat4_orthographic3(-5, 5, 3, -2, 0.0001f, 100.0f);
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

TEST_PROC(maths__quaternion__q_inverse_equals_q_conj_if_q_unit)
{
    Quaternion q = quat_angle_axis(f32_PI/2.0f, { 0, 0, 1 });
    ASSERT(almost_equal(length(q), 1));
    ASSERT(almost_equal(quat_conjugate(q), quat_inverse(q)));
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

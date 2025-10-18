#ifndef MATHS_TEST_H
#define MATHS_TEST_H

extern void maths__vector2__operators();
extern void maths__foobar();
extern void maths__vector2__swizzle();
extern void maths__vector3__operators();
extern void maths__vector3__vec_mul_vec_equals_component_wise_mul();
extern void maths__vector3__vec_div_vec_equals_component_wise_div();
extern void maths__vector3__swizzle();
extern void maths__vector3__dot_of_orthogonal_vectors_is_0();
extern void maths__vector3__cross_of_orthogonal_ijk_is_plus_minus_kji();
extern void maths__vector3__cross_of_zero_vector_is_always_zero();
extern void maths__vector3__cross_is_anti_commutative();
extern void maths__vector3__cross_distributive_law();
extern void maths__vector3__cross_scalar_factorisation();
extern void maths__vector3__vector_triple_product_abc_is_bac_minus_cab();
extern void maths__vector3__a_cross_b_squared_is_lagranges_identity();
extern void maths__vector3__scalar_triple_product_of_ijk_is_1();
extern void maths__vector3__a_plus_b_satisfies_triangle_inequality();
extern void maths__vector3__a_minus_b_satisfies_reverse_triangle_inequality();
extern void maths__vector4__operators();
extern void maths__vector4__vec_mul_vec_equals_component_wise_mul();
extern void maths__vector4__vec_div_vec_equals_component_wise_div();
extern void maths__vector4__swizzle();
extern void maths__mat3__rotate_x();
extern void maths__mat3__rotate_y();
extern void maths__mat3__rotate_z();
extern void maths__mat3__rotate_axis();
extern void maths__mat3__determinant_of_identity_is_1();
extern void maths__mat3__determinant_of_M_equals_determinant_of_M_transposed();
extern void maths__mat3__M_times_M_inverse_equals_identity();
extern void maths__mat4__M_times_M_inverse_equals_identity();
extern void maths__quaternion__constructors();
extern void maths__quaternion__swizzle();
extern void maths__quaternion__angle_axis();
extern void maths__quaternion__euler();
extern void maths__quaternion__inverse();
extern void maths__quaternion__add_is_componentwise();
extern void maths__quaternion__add_is_associative();
extern void maths__quaternion__add_is_commutative();
extern void maths__quaternion__scalar_mul_quat_is_distributive();
extern void maths__quaternion__mul_is_associative();
extern void maths__quaternion__mul_is_distributive();
extern void maths__quaternion__q2_mul_q1_equals_q1q2_minus_2v1_cross_v2();
extern void maths__quaternion__q_mul_q_conj_is_commutative();
extern void maths__quaternion__q_mul_q_conj_equals_v_sq_plus_s_sq();
extern void maths__quaternion__q_inverse_equals_q_conj_if_q_unit();
extern void maths__misc__angle_between();

TestSuite MATHS__maths__vector2__tests[] = {
	{ "operators", maths__vector2__operators },
	{ "swizzle", maths__vector2__swizzle },
};

TestSuite MATHS__maths__tests[] = {
	{ "foobar", maths__foobar },
};

TestSuite MATHS__maths__vector3__tests[] = {
	{ "operators", maths__vector3__operators },
	{ "vec_mul_vec_equals_component_wise_mul", maths__vector3__vec_mul_vec_equals_component_wise_mul },
	{ "vec_div_vec_equals_component_wise_div", maths__vector3__vec_div_vec_equals_component_wise_div },
	{ "swizzle", maths__vector3__swizzle },
	{ "dot_of_orthogonal_vectors_is_0", maths__vector3__dot_of_orthogonal_vectors_is_0 },
	{ "cross_of_orthogonal_ijk_is_plus_minus_kji", maths__vector3__cross_of_orthogonal_ijk_is_plus_minus_kji },
	{ "cross_of_zero_vector_is_always_zero", maths__vector3__cross_of_zero_vector_is_always_zero },
	{ "cross_is_anti_commutative", maths__vector3__cross_is_anti_commutative },
	{ "cross_distributive_law", maths__vector3__cross_distributive_law },
	{ "cross_scalar_factorisation", maths__vector3__cross_scalar_factorisation },
	{ "vector_triple_product_abc_is_bac_minus_cab", maths__vector3__vector_triple_product_abc_is_bac_minus_cab },
	{ "a_cross_b_squared_is_lagranges_identity", maths__vector3__a_cross_b_squared_is_lagranges_identity },
	{ "scalar_triple_product_of_ijk_is_1", maths__vector3__scalar_triple_product_of_ijk_is_1 },
	{ "a_plus_b_satisfies_triangle_inequality", maths__vector3__a_plus_b_satisfies_triangle_inequality },
	{ "a_minus_b_satisfies_reverse_triangle_inequality", maths__vector3__a_minus_b_satisfies_reverse_triangle_inequality },
};

TestSuite MATHS__maths__vector4__tests[] = {
	{ "operators", maths__vector4__operators },
	{ "vec_mul_vec_equals_component_wise_mul", maths__vector4__vec_mul_vec_equals_component_wise_mul },
	{ "vec_div_vec_equals_component_wise_div", maths__vector4__vec_div_vec_equals_component_wise_div },
	{ "swizzle", maths__vector4__swizzle },
};

TestSuite MATHS__maths__mat3__tests[] = {
	{ "rotate_x", maths__mat3__rotate_x },
	{ "rotate_y", maths__mat3__rotate_y },
	{ "rotate_z", maths__mat3__rotate_z },
	{ "rotate_axis", maths__mat3__rotate_axis },
	{ "determinant_of_identity_is_1", maths__mat3__determinant_of_identity_is_1 },
	{ "determinant_of_M_equals_determinant_of_M_transposed", maths__mat3__determinant_of_M_equals_determinant_of_M_transposed },
	{ "M_times_M_inverse_equals_identity", maths__mat3__M_times_M_inverse_equals_identity },
};

TestSuite MATHS__maths__mat4__tests[] = {
	{ "M_times_M_inverse_equals_identity", maths__mat4__M_times_M_inverse_equals_identity },
};

TestSuite MATHS__maths__quaternion__tests[] = {
	{ "constructors", maths__quaternion__constructors },
	{ "swizzle", maths__quaternion__swizzle },
	{ "angle_axis", maths__quaternion__angle_axis },
	{ "euler", maths__quaternion__euler },
	{ "inverse", maths__quaternion__inverse },
	{ "add_is_componentwise", maths__quaternion__add_is_componentwise },
	{ "add_is_associative", maths__quaternion__add_is_associative },
	{ "add_is_commutative", maths__quaternion__add_is_commutative },
	{ "scalar_mul_quat_is_distributive", maths__quaternion__scalar_mul_quat_is_distributive },
	{ "mul_is_associative", maths__quaternion__mul_is_associative },
	{ "mul_is_distributive", maths__quaternion__mul_is_distributive },
	{ "q2_mul_q1_equals_q1q2_minus_2v1_cross_v2", maths__quaternion__q2_mul_q1_equals_q1q2_minus_2v1_cross_v2 },
	{ "q_mul_q_conj_is_commutative", maths__quaternion__q_mul_q_conj_is_commutative },
	{ "q_mul_q_conj_equals_v_sq_plus_s_sq", maths__quaternion__q_mul_q_conj_equals_v_sq_plus_s_sq },
	{ "q_inverse_equals_q_conj_if_q_unit", maths__quaternion__q_inverse_equals_q_conj_if_q_unit },
};

TestSuite MATHS__maths__misc__tests[] = {
	{ "angle_between", maths__misc__angle_between },
};

TestSuite MATHS__tests[] = {
	{ "maths", nullptr, MATHS__maths__tests, sizeof(MATHS__maths__tests)/sizeof(MATHS__maths__tests[0]) },
	{ "maths/mat3", nullptr, MATHS__maths__mat3__tests, sizeof(MATHS__maths__mat3__tests)/sizeof(MATHS__maths__mat3__tests[0]) },
	{ "maths/mat4", nullptr, MATHS__maths__mat4__tests, sizeof(MATHS__maths__mat4__tests)/sizeof(MATHS__maths__mat4__tests[0]) },
	{ "maths/misc", nullptr, MATHS__maths__misc__tests, sizeof(MATHS__maths__misc__tests)/sizeof(MATHS__maths__misc__tests[0]) },
	{ "maths/quaternion", nullptr, MATHS__maths__quaternion__tests, sizeof(MATHS__maths__quaternion__tests)/sizeof(MATHS__maths__quaternion__tests[0]) },
	{ "maths/vector2", nullptr, MATHS__maths__vector2__tests, sizeof(MATHS__maths__vector2__tests)/sizeof(MATHS__maths__vector2__tests[0]) },
	{ "maths/vector3", nullptr, MATHS__maths__vector3__tests, sizeof(MATHS__maths__vector3__tests)/sizeof(MATHS__maths__vector3__tests[0]) },
	{ "maths/vector4", nullptr, MATHS__maths__vector4__tests, sizeof(MATHS__maths__vector4__tests)/sizeof(MATHS__maths__vector4__tests[0]) },
};

#endif // MATHS_TEST_H

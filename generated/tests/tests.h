#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void array__indexing_test();
static void array__iterator_test();
static void dynamic_array__append_test();
static void dynamic_array__insert_test();
static void dynamic_array__set_test();
static void dynamic_array__reserve_test();
static void dynamic_array__resize_test();
static void fixed_array__basic_construction_test();
static void fixed_array__copy_operations_test();
static void fixed_array__copy_invokes_copy_constructor_for_each_element_test();
static void fixed_array__size_limits_test();
static void fixed_array__data_pointer_integrity_test();
static void array__pop_test();
static void array__tail_test();
static void array__create_test();
static void array__remove_test();
static void array__slice_test();
static void array__find_test();
static void array__swap_test();
static void array__sort_test();
extern void maths__vector2__operators_test();
extern void maths__vector2__swizzle_test();
extern void maths__vector3__operators_test();
extern void maths__vector3__vec_mul_vec_equals_component_wise_mul_test();
extern void maths__vector3__vec_div_vec_equals_component_wise_div_test();
extern void maths__vector3__swizzle_test();
extern void maths__vector3__dot_of_orthogonal_vectors_is_0_test();
extern void maths__vector3__cross_of_orthogonal_ijk_is_plus_minus_kji_test();
extern void maths__vector3__cross_of_zero_vector_is_always_zero_test();
extern void maths__vector3__cross_is_anti_commutative_test();
extern void maths__vector3__cross_distributive_law_test();
extern void maths__vector3__cross_scalar_factorisation_test();
extern void maths__vector3__vector_triple_product_abc_is_bac_minus_cab_test();
extern void maths__vector3__a_cross_b_squared_is_lagranges_identity_test();
extern void maths__vector3__scalar_triple_product_of_ijk_is_1_test();
extern void maths__vector3__a_plus_b_satisfies_triangle_inequality_test();
extern void maths__vector3__a_minus_b_satisfies_reverse_triangle_inequality_test();
extern void maths__vector4__operators_test();
extern void maths__vector4__vec_mul_vec_equals_component_wise_mul_test();
extern void maths__vector4__vec_div_vec_equals_component_wise_div_test();
extern void maths__vector4__swizzle_test();
extern void maths__mat3__rotate_x_test();
extern void maths__mat3__rotate_y_test();
extern void maths__mat3__rotate_z_test();
extern void maths__mat3__rotate_axis_test();
extern void maths__mat3__determinant_of_identity_is_1_test();
extern void maths__mat3__determinant_of_M_equals_determinant_of_M_transposed_test();
extern void maths__mat3__M_times_M_inverse_equals_identity_test();
extern void maths__mat4__M_times_M_inverse_equals_identity_test();
extern void maths__quaternion__constructors_test();
extern void maths__quaternion__swizzle_test();
extern void maths__quaternion__angle_axis_test();
extern void maths__quaternion__euler_test();
extern void maths__quaternion__inverse_test();
extern void maths__quaternion__add_is_componentwise_test();
extern void maths__quaternion__add_is_associative_test();
extern void maths__quaternion__add_is_commutative_test();
extern void maths__quaternion__scalar_mul_quat_is_distributive_test();
extern void maths__quaternion__mul_is_associative_test();
extern void maths__quaternion__mul_is_distributive_test();
extern void maths__quaternion__q2_mul_q1_equals_q1q2_minus_2v1_cross_v2_test();
extern void maths__quaternion__q_mul_q_conj_is_commutative_test();
extern void maths__quaternion__q_mul_q_conj_equals_v_sq_plus_s_sq_test();
extern void maths__quaternion__q_inverse_equals_q_conj_if_q_unit_test();
extern void maths__misc__angle_between_test();

TestSuite TESTS_array_tests[] = {
	{ "indexing_test", array__indexing_test },
	{ "iterator_test", array__iterator_test },
	{ "pop_test", array__pop_test },
	{ "tail_test", array__tail_test },
	{ "create_test", array__create_test },
	{ "remove_test", array__remove_test },
	{ "slice_test", array__slice_test },
	{ "find_test", array__find_test },
	{ "swap_test", array__swap_test },
	{ "sort_test", array__sort_test },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "append_test", dynamic_array__append_test },
	{ "insert_test", dynamic_array__insert_test },
	{ "set_test", dynamic_array__set_test },
	{ "reserve_test", dynamic_array__reserve_test },
	{ "resize_test", dynamic_array__resize_test },
};
TestSuite TESTS_fixed_array_tests[] = {
	{ "basic_construction_test", fixed_array__basic_construction_test },
	{ "copy_operations_test", fixed_array__copy_operations_test },
	{ "copy_invokes_copy_constructor_for_each_element_test", fixed_array__copy_invokes_copy_constructor_for_each_element_test },
	{ "size_limits_test", fixed_array__size_limits_test },
	{ "data_pointer_integrity_test", fixed_array__data_pointer_integrity_test },
};
TestSuite TESTS_maths__vector2_tests[] = {
	{ "operators_test", maths__vector2__operators_test },
	{ "swizzle_test", maths__vector2__swizzle_test },
};
TestSuite TESTS_maths__vector3_tests[] = {
	{ "operators_test", maths__vector3__operators_test },
	{ "vec_mul_vec_equals_component_wise_mul_test", maths__vector3__vec_mul_vec_equals_component_wise_mul_test },
	{ "vec_div_vec_equals_component_wise_div_test", maths__vector3__vec_div_vec_equals_component_wise_div_test },
	{ "swizzle_test", maths__vector3__swizzle_test },
	{ "dot_of_orthogonal_vectors_is_0_test", maths__vector3__dot_of_orthogonal_vectors_is_0_test },
	{ "cross_of_orthogonal_ijk_is_plus_minus_kji_test", maths__vector3__cross_of_orthogonal_ijk_is_plus_minus_kji_test },
	{ "cross_of_zero_vector_is_always_zero_test", maths__vector3__cross_of_zero_vector_is_always_zero_test },
	{ "cross_is_anti_commutative_test", maths__vector3__cross_is_anti_commutative_test },
	{ "cross_distributive_law_test", maths__vector3__cross_distributive_law_test },
	{ "cross_scalar_factorisation_test", maths__vector3__cross_scalar_factorisation_test },
	{ "vector_triple_product_abc_is_bac_minus_cab_test", maths__vector3__vector_triple_product_abc_is_bac_minus_cab_test },
	{ "a_cross_b_squared_is_lagranges_identity_test", maths__vector3__a_cross_b_squared_is_lagranges_identity_test },
	{ "scalar_triple_product_of_ijk_is_1_test", maths__vector3__scalar_triple_product_of_ijk_is_1_test },
	{ "a_plus_b_satisfies_triangle_inequality_test", maths__vector3__a_plus_b_satisfies_triangle_inequality_test },
	{ "a_minus_b_satisfies_reverse_triangle_inequality_test", maths__vector3__a_minus_b_satisfies_reverse_triangle_inequality_test },
};
TestSuite TESTS_maths__vector4_tests[] = {
	{ "operators_test", maths__vector4__operators_test },
	{ "vec_mul_vec_equals_component_wise_mul_test", maths__vector4__vec_mul_vec_equals_component_wise_mul_test },
	{ "vec_div_vec_equals_component_wise_div_test", maths__vector4__vec_div_vec_equals_component_wise_div_test },
	{ "swizzle_test", maths__vector4__swizzle_test },
};
TestSuite TESTS_maths__mat3_tests[] = {
	{ "rotate_x_test", maths__mat3__rotate_x_test },
	{ "rotate_y_test", maths__mat3__rotate_y_test },
	{ "rotate_z_test", maths__mat3__rotate_z_test },
	{ "rotate_axis_test", maths__mat3__rotate_axis_test },
	{ "determinant_of_identity_is_1_test", maths__mat3__determinant_of_identity_is_1_test },
	{ "determinant_of_M_equals_determinant_of_M_transposed_test", maths__mat3__determinant_of_M_equals_determinant_of_M_transposed_test },
	{ "M_times_M_inverse_equals_identity_test", maths__mat3__M_times_M_inverse_equals_identity_test },
};
TestSuite TESTS_maths__mat4_tests[] = {
	{ "M_times_M_inverse_equals_identity_test", maths__mat4__M_times_M_inverse_equals_identity_test },
};
TestSuite TESTS_maths__quaternion_tests[] = {
	{ "constructors_test", maths__quaternion__constructors_test },
	{ "swizzle_test", maths__quaternion__swizzle_test },
	{ "angle_axis_test", maths__quaternion__angle_axis_test },
	{ "euler_test", maths__quaternion__euler_test },
	{ "inverse_test", maths__quaternion__inverse_test },
	{ "add_is_componentwise_test", maths__quaternion__add_is_componentwise_test },
	{ "add_is_associative_test", maths__quaternion__add_is_associative_test },
	{ "add_is_commutative_test", maths__quaternion__add_is_commutative_test },
	{ "scalar_mul_quat_is_distributive_test", maths__quaternion__scalar_mul_quat_is_distributive_test },
	{ "mul_is_associative_test", maths__quaternion__mul_is_associative_test },
	{ "mul_is_distributive_test", maths__quaternion__mul_is_distributive_test },
	{ "q2_mul_q1_equals_q1q2_minus_2v1_cross_v2_test", maths__quaternion__q2_mul_q1_equals_q1q2_minus_2v1_cross_v2_test },
	{ "q_mul_q_conj_is_commutative_test", maths__quaternion__q_mul_q_conj_is_commutative_test },
	{ "q_mul_q_conj_equals_v_sq_plus_s_sq_test", maths__quaternion__q_mul_q_conj_equals_v_sq_plus_s_sq_test },
	{ "q_inverse_equals_q_conj_if_q_unit_test", maths__quaternion__q_inverse_equals_q_conj_if_q_unit_test },
};
TestSuite TESTS_maths__misc_tests[] = {
	{ "angle_between_test", maths__misc__angle_between_test },
};
TestSuite TESTS_tests[] = {
	{ "array", nullptr, TESTS_array_tests, sizeof(TESTS_array_tests)/sizeof(TESTS_array_tests[0]) },
	{ "dynamic_array", nullptr, TESTS_dynamic_array_tests, sizeof(TESTS_dynamic_array_tests)/sizeof(TESTS_dynamic_array_tests[0]) },
	{ "fixed_array", nullptr, TESTS_fixed_array_tests, sizeof(TESTS_fixed_array_tests)/sizeof(TESTS_fixed_array_tests[0]) },
	{ "maths__vector2", nullptr, TESTS_maths__vector2_tests, sizeof(TESTS_maths__vector2_tests)/sizeof(TESTS_maths__vector2_tests[0]) },
	{ "maths__vector3", nullptr, TESTS_maths__vector3_tests, sizeof(TESTS_maths__vector3_tests)/sizeof(TESTS_maths__vector3_tests[0]) },
	{ "maths__vector4", nullptr, TESTS_maths__vector4_tests, sizeof(TESTS_maths__vector4_tests)/sizeof(TESTS_maths__vector4_tests[0]) },
	{ "maths__mat3", nullptr, TESTS_maths__mat3_tests, sizeof(TESTS_maths__mat3_tests)/sizeof(TESTS_maths__mat3_tests[0]) },
	{ "maths__mat4", nullptr, TESTS_maths__mat4_tests, sizeof(TESTS_maths__mat4_tests)/sizeof(TESTS_maths__mat4_tests[0]) },
	{ "maths__quaternion", nullptr, TESTS_maths__quaternion_tests, sizeof(TESTS_maths__quaternion_tests)/sizeof(TESTS_maths__quaternion_tests[0]) },
	{ "maths__misc", nullptr, TESTS_maths__misc_tests, sizeof(TESTS_maths__misc_tests)/sizeof(TESTS_maths__misc_tests[0]) },
};

#endif // TESTS_TEST_H

#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void array__indexing();
static void array__iterator();
static void dynamic_array__copy_replaces_contents();
static void dynamic_array__append();
static void dynamic_array__insert();
static void dynamic_array__set();
static void dynamic_array__reserve();
static void dynamic_array__resize();
static void fixed_array__basic_construction();
static void fixed_array__copy_operations();
static void fixed_array__copy_invokes_copy_constructor_for_each_element();
extern void fixed_array__self_assign_avoids_copies();
static void fixed_array__size_limits();
static void fixed_array__data_pointer_integrity();
static void array__pop();
static void array__tail();
static void array__create();
static void array__remove();
static void array__slice();
static void array__find();
static void array__swap();
static void array__sort();
extern void maths__vector2__operators();
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
static void dynamic_map__set_invokes_copy_constructor_for_key_and_value();
static void dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key();

TestSuite TESTS_array_tests[] = {
	{ "indexing", array__indexing },
	{ "iterator", array__iterator },
	{ "pop", array__pop },
	{ "tail", array__tail },
	{ "create", array__create },
	{ "remove", array__remove },
	{ "slice", array__slice },
	{ "find", array__find },
	{ "swap", array__swap },
	{ "sort", array__sort },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "copy_replaces_contents", dynamic_array__copy_replaces_contents },
	{ "append", dynamic_array__append },
	{ "insert", dynamic_array__insert },
	{ "set", dynamic_array__set },
	{ "reserve", dynamic_array__reserve },
	{ "resize", dynamic_array__resize },
};
TestSuite TESTS_fixed_array_tests[] = {
	{ "basic_construction", fixed_array__basic_construction },
	{ "copy_operations", fixed_array__copy_operations },
	{ "copy_invokes_copy_constructor_for_each_element", fixed_array__copy_invokes_copy_constructor_for_each_element },
	{ "self_assign_avoids_copies", fixed_array__self_assign_avoids_copies },
	{ "size_limits", fixed_array__size_limits },
	{ "data_pointer_integrity", fixed_array__data_pointer_integrity },
};
TestSuite TESTS_maths__vector2_tests[] = {
	{ "operators", maths__vector2__operators },
	{ "swizzle", maths__vector2__swizzle },
};
TestSuite TESTS_maths__vector3_tests[] = {
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
TestSuite TESTS_maths__vector4_tests[] = {
	{ "operators", maths__vector4__operators },
	{ "vec_mul_vec_equals_component_wise_mul", maths__vector4__vec_mul_vec_equals_component_wise_mul },
	{ "vec_div_vec_equals_component_wise_div", maths__vector4__vec_div_vec_equals_component_wise_div },
	{ "swizzle", maths__vector4__swizzle },
};
TestSuite TESTS_maths__mat3_tests[] = {
	{ "rotate_x", maths__mat3__rotate_x },
	{ "rotate_y", maths__mat3__rotate_y },
	{ "rotate_z", maths__mat3__rotate_z },
	{ "rotate_axis", maths__mat3__rotate_axis },
	{ "determinant_of_identity_is_1", maths__mat3__determinant_of_identity_is_1 },
	{ "determinant_of_M_equals_determinant_of_M_transposed", maths__mat3__determinant_of_M_equals_determinant_of_M_transposed },
	{ "M_times_M_inverse_equals_identity", maths__mat3__M_times_M_inverse_equals_identity },
};
TestSuite TESTS_maths__mat4_tests[] = {
	{ "M_times_M_inverse_equals_identity", maths__mat4__M_times_M_inverse_equals_identity },
};
TestSuite TESTS_maths__quaternion_tests[] = {
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
TestSuite TESTS_maths__misc_tests[] = {
	{ "angle_between", maths__misc__angle_between },
};
TestSuite TESTS_dynamic_map_tests[] = {
	{ "set_invokes_copy_constructor_for_key_and_value", dynamic_map__set_invokes_copy_constructor_for_key_and_value },
	{ "set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key", dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key },
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
	{ "dynamic_map", nullptr, TESTS_dynamic_map_tests, sizeof(TESTS_dynamic_map_tests)/sizeof(TESTS_dynamic_map_tests[0]) },
};

#endif // TESTS_TEST_H

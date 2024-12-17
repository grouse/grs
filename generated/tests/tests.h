#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void indexing_test0();
static void find_test1();
static void iterator_test2();
static void append_test3();
static void insert_test4();
static void set_test5();
static void reserve_test6();
static void resize_test7();
static void basic_construction_test8();
static void copy_operations_test9();
static void size_limits_test10();
static void data_pointer_integrity_test11();
static void pop_test12();
static void tail_test13();
static void create_test14();
static void remove_test15();
static void slice_test16();
static void find_test17();
static void swap_test18();
static void sort_test19();
extern void operators_test20();
extern void swizzle_test21();
extern void operators_test22();
extern void vec_mul_vec_equals_component_wise_mul_test23();
extern void vec_div_vec_equals_component_wise_div_test24();
extern void swizzle_test25();
extern void dot_of_orthogonal_vectors_is_0_test26();
extern void cross_of_orthogonal_ijk_is_plus_minus_kji_test27();
extern void cross_of_zero_vector_is_always_zero_test28();
extern void cross_is_anti_commutative_test29();
extern void cross_distributive_law_test30();
extern void cross_scalar_factorisation_test31();
extern void vector_triple_product_abc_is_bac_minus_cab_test32();
extern void a_cross_b_squared_is_lagranges_identity_test33();
extern void scalar_triple_product_of_ijk_is_1_test34();
extern void a_plus_b_satisfies_triangle_inequality_test35();
extern void a_minus_b_satisfies_reverse_triangle_inequality_test36();
extern void operators_test37();
extern void vec_mul_vec_equals_component_wise_mul_test38();
extern void vec_div_vec_equals_component_wise_div_test39();
extern void swizzle_test40();
extern void determinant_of_identity_is_1_test41();
extern void rotate_x_test42();
extern void rotate_y_test43();
extern void rotate_z_test44();
extern void rotate_axis_test45();
extern void determinant_of_M_equals_determinant_of_M_transposed_test46();
extern void M_times_M_inverse_equals_identity_test47();
extern void M_times_M_inverse_equals_identity_test48();
extern void constructors_test49();
extern void swizzle_test50();
extern void angle_axis_test51();
extern void euler_test52();
extern void inverse_test53();
extern void angle_between_test54();

TestSuite TESTS_array_tests[] = {
	{ "indexing", indexing_test0 },
	{ "find", find_test1 },
	{ "iterator", iterator_test2 },
	{ "pop", pop_test12 },
	{ "tail", tail_test13 },
	{ "create", create_test14 },
	{ "remove", remove_test15 },
	{ "slice", slice_test16 },
	{ "find", find_test17 },
	{ "swap", swap_test18 },
	{ "sort", sort_test19 },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "append", append_test3 },
	{ "insert", insert_test4 },
	{ "set", set_test5 },
	{ "reserve", reserve_test6 },
	{ "resize", resize_test7 },
};
TestSuite TESTS_fixed_array_tests[] = {
	{ "basic_construction", basic_construction_test8 },
	{ "copy_operations", copy_operations_test9 },
	{ "size_limits", size_limits_test10 },
	{ "data_pointer_integrity", data_pointer_integrity_test11 },
};
TestSuite TESTS_vector2_tests[] = {
	{ "operators", operators_test20 },
	{ "swizzle", swizzle_test21 },
};
TestSuite TESTS_vector3_tests[] = {
	{ "operators", operators_test22 },
	{ "vec_mul_vec_equals_component_wise_mul", vec_mul_vec_equals_component_wise_mul_test23 },
	{ "vec_div_vec_equals_component_wise_div", vec_div_vec_equals_component_wise_div_test24 },
	{ "swizzle", swizzle_test25 },
	{ "dot_of_orthogonal_vectors_is_0", dot_of_orthogonal_vectors_is_0_test26 },
	{ "cross_of_orthogonal_ijk_is_plus_minus_kji", cross_of_orthogonal_ijk_is_plus_minus_kji_test27 },
	{ "cross_of_zero_vector_is_always_zero", cross_of_zero_vector_is_always_zero_test28 },
	{ "cross_is_anti_commutative", cross_is_anti_commutative_test29 },
	{ "cross_distributive_law", cross_distributive_law_test30 },
	{ "cross_scalar_factorisation", cross_scalar_factorisation_test31 },
	{ "vector_triple_product_abc_is_bac_minus_cab", vector_triple_product_abc_is_bac_minus_cab_test32 },
	{ "a_cross_b_squared_is_lagranges_identity", a_cross_b_squared_is_lagranges_identity_test33 },
	{ "scalar_triple_product_of_ijk_is_1", scalar_triple_product_of_ijk_is_1_test34 },
	{ "a_plus_b_satisfies_triangle_inequality", a_plus_b_satisfies_triangle_inequality_test35 },
	{ "a_minus_b_satisfies_reverse_triangle_inequality", a_minus_b_satisfies_reverse_triangle_inequality_test36 },
};
TestSuite TESTS_vector4_tests[] = {
	{ "operators", operators_test37 },
	{ "vec_mul_vec_equals_component_wise_mul", vec_mul_vec_equals_component_wise_mul_test38 },
	{ "vec_div_vec_equals_component_wise_div", vec_div_vec_equals_component_wise_div_test39 },
	{ "swizzle", swizzle_test40 },
};
TestSuite TESTS_maths_mat3_tests[] = {
	{ "determinant_of_identity_is_1", determinant_of_identity_is_1_test41 },
	{ "rotate_x", rotate_x_test42 },
	{ "rotate_y", rotate_y_test43 },
	{ "rotate_z", rotate_z_test44 },
	{ "rotate_axis", rotate_axis_test45 },
	{ "determinant_of_M_equals_determinant_of_M_transposed", determinant_of_M_equals_determinant_of_M_transposed_test46 },
	{ "M_times_M_inverse_equals_identity", M_times_M_inverse_equals_identity_test47 },
};
TestSuite TESTS_maths_mat4_tests[] = {
	{ "M_times_M_inverse_equals_identity", M_times_M_inverse_equals_identity_test48 },
};
TestSuite TESTS_maths_quaternion_tests[] = {
	{ "constructors", constructors_test49 },
	{ "swizzle", swizzle_test50 },
	{ "angle_axis", angle_axis_test51 },
	{ "euler", euler_test52 },
	{ "inverse", inverse_test53 },
};
TestSuite TESTS_maths_misc_tests[] = {
	{ "angle_between", angle_between_test54 },
};
TestSuite TESTS_tests[] = {
	{ "array", nullptr, TESTS_array_tests, sizeof(TESTS_array_tests)/sizeof(TESTS_array_tests[0]) },
	{ "dynamic_array", nullptr, TESTS_dynamic_array_tests, sizeof(TESTS_dynamic_array_tests)/sizeof(TESTS_dynamic_array_tests[0]) },
	{ "fixed_array", nullptr, TESTS_fixed_array_tests, sizeof(TESTS_fixed_array_tests)/sizeof(TESTS_fixed_array_tests[0]) },
	{ "vector2", nullptr, TESTS_vector2_tests, sizeof(TESTS_vector2_tests)/sizeof(TESTS_vector2_tests[0]) },
	{ "vector3", nullptr, TESTS_vector3_tests, sizeof(TESTS_vector3_tests)/sizeof(TESTS_vector3_tests[0]) },
	{ "vector4", nullptr, TESTS_vector4_tests, sizeof(TESTS_vector4_tests)/sizeof(TESTS_vector4_tests[0]) },
	{ "maths_mat3", nullptr, TESTS_maths_mat3_tests, sizeof(TESTS_maths_mat3_tests)/sizeof(TESTS_maths_mat3_tests[0]) },
	{ "maths_mat4", nullptr, TESTS_maths_mat4_tests, sizeof(TESTS_maths_mat4_tests)/sizeof(TESTS_maths_mat4_tests[0]) },
	{ "maths_quaternion", nullptr, TESTS_maths_quaternion_tests, sizeof(TESTS_maths_quaternion_tests)/sizeof(TESTS_maths_quaternion_tests[0]) },
	{ "maths_misc", nullptr, TESTS_maths_misc_tests, sizeof(TESTS_maths_misc_tests)/sizeof(TESTS_maths_misc_tests[0]) },
};

#endif // TESTS_TEST_H

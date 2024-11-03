#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void indexing_test0();
static void find_test1();
static void iterator_test2();
static void append_test3();
static void insert_test4();
static void set_test5();
static void basic_construction_test6();
static void copy_operations_test7();
static void size_limits_test8();
static void data_pointer_integrity_test9();
static void pop_test10();
static void tail_test11();
static void create_test12();
static void remove_test13();
static void slice_test14();
static void find_test15();
static void swap_test16();
static void sort_test17();
extern void operators_test18();
extern void swizzle_test19();
extern void operators_test20();
extern void swizzle_test21();
extern void operators_test22();
extern void swizzle_test23();
extern void constructors_test24();
extern void swizzle_test25();
extern void angle_axis_test26();
extern void euler_test27();
extern void inverse_test28();
extern void angle_between_test29();

TestSuite TESTS_array_tests[] = {
	{ "indexing", indexing_test0 },
	{ "find", find_test1 },
	{ "iterator", iterator_test2 },
	{ "pop", pop_test10 },
	{ "tail", tail_test11 },
	{ "create", create_test12 },
	{ "remove", remove_test13 },
	{ "slice", slice_test14 },
	{ "find", find_test15 },
	{ "swap", swap_test16 },
	{ "sort", sort_test17 },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "append", append_test3 },
	{ "insert", insert_test4 },
	{ "set", set_test5 },
};
TestSuite TESTS_fixed_array_tests[] = {
	{ "basic_construction", basic_construction_test6 },
	{ "copy_operations", copy_operations_test7 },
	{ "size_limits", size_limits_test8 },
	{ "data_pointer_integrity", data_pointer_integrity_test9 },
};
TestSuite TESTS_vector2_tests[] = {
	{ "operators", operators_test18 },
	{ "swizzle", swizzle_test19 },
};
TestSuite TESTS_vector3_tests[] = {
	{ "operators", operators_test20 },
	{ "swizzle", swizzle_test21 },
};
TestSuite TESTS_vector4_tests[] = {
	{ "operators", operators_test22 },
	{ "swizzle", swizzle_test23 },
};
TestSuite TESTS_maths_quaternion_tests[] = {
	{ "constructors", constructors_test24 },
	{ "swizzle", swizzle_test25 },
	{ "angle_axis", angle_axis_test26 },
	{ "euler", euler_test27 },
	{ "inverse", inverse_test28 },
};
TestSuite TESTS_maths_misc_tests[] = {
	{ "angle_between", angle_between_test29 },
};
TestSuite TESTS_tests[] = {
	{ "array", nullptr, TESTS_array_tests, sizeof(TESTS_array_tests)/sizeof(TESTS_array_tests[0]) },
	{ "dynamic_array", nullptr, TESTS_dynamic_array_tests, sizeof(TESTS_dynamic_array_tests)/sizeof(TESTS_dynamic_array_tests[0]) },
	{ "fixed_array", nullptr, TESTS_fixed_array_tests, sizeof(TESTS_fixed_array_tests)/sizeof(TESTS_fixed_array_tests[0]) },
	{ "vector2", nullptr, TESTS_vector2_tests, sizeof(TESTS_vector2_tests)/sizeof(TESTS_vector2_tests[0]) },
	{ "vector3", nullptr, TESTS_vector3_tests, sizeof(TESTS_vector3_tests)/sizeof(TESTS_vector3_tests[0]) },
	{ "vector4", nullptr, TESTS_vector4_tests, sizeof(TESTS_vector4_tests)/sizeof(TESTS_vector4_tests[0]) },
	{ "maths_quaternion", nullptr, TESTS_maths_quaternion_tests, sizeof(TESTS_maths_quaternion_tests)/sizeof(TESTS_maths_quaternion_tests[0]) },
	{ "maths_misc", nullptr, TESTS_maths_misc_tests, sizeof(TESTS_maths_misc_tests)/sizeof(TESTS_maths_misc_tests[0]) },
};

#endif // TESTS_TEST_H

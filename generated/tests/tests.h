#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void indexing_test0();
static void removal_test1();
static void find_test2();
static void iterator_test3();
static void append_test4();
static void insert_test5();
static void set_test6();
static void basic_construction_test7();
static void copy_operations_test8();
static void size_limits_test9();
static void data_pointer_integrity_test10();
static void pop_test11();
static void tail_test12();
static void create_test13();
static void remove_test14();
static void slice_test15();
static void find_test16();
static void swap_test17();
static void sort_test18();
extern void operators_test19();
extern void swizzle_test20();
extern void operators_test21();
extern void swizzle_test22();
extern void operators_test23();
extern void swizzle_test24();
extern void constructors_test25();
extern void swizzle_test26();
extern void angle_axis_test27();
extern void euler_test28();
extern void inverse_test29();
extern void angle_between_test30();

TestSuite TESTS_array_tests[] = {
	{ "indexing", indexing_test0 },
	{ "removal", removal_test1 },
	{ "find", find_test2 },
	{ "iterator", iterator_test3 },
	{ "pop", pop_test11 },
	{ "tail", tail_test12 },
	{ "create", create_test13 },
	{ "remove", remove_test14 },
	{ "slice", slice_test15 },
	{ "find", find_test16 },
	{ "swap", swap_test17 },
	{ "sort", sort_test18 },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "append", append_test4 },
	{ "insert", insert_test5 },
	{ "set", set_test6 },
};
TestSuite TESTS_fixed_array_tests[] = {
	{ "basic_construction", basic_construction_test7 },
	{ "copy_operations", copy_operations_test8 },
	{ "size_limits", size_limits_test9 },
	{ "data_pointer_integrity", data_pointer_integrity_test10 },
};
TestSuite TESTS_vector2_tests[] = {
	{ "operators", operators_test19 },
	{ "swizzle", swizzle_test20 },
};
TestSuite TESTS_vector3_tests[] = {
	{ "operators", operators_test21 },
	{ "swizzle", swizzle_test22 },
};
TestSuite TESTS_vector4_tests[] = {
	{ "operators", operators_test23 },
	{ "swizzle", swizzle_test24 },
};
TestSuite TESTS_maths_quaternion_tests[] = {
	{ "constructors", constructors_test25 },
	{ "swizzle", swizzle_test26 },
	{ "angle_axis", angle_axis_test27 },
	{ "euler", euler_test28 },
	{ "inverse", inverse_test29 },
};
TestSuite TESTS_maths_misc_tests[] = {
	{ "angle_between", angle_between_test30 },
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

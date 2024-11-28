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
extern void swizzle_test23();
extern void operators_test24();
extern void swizzle_test25();
extern void constructors_test26();
extern void swizzle_test27();
extern void angle_axis_test28();
extern void euler_test29();
extern void inverse_test30();
extern void angle_between_test31();

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
	{ "swizzle", swizzle_test23 },
};
TestSuite TESTS_vector4_tests[] = {
	{ "operators", operators_test24 },
	{ "swizzle", swizzle_test25 },
};
TestSuite TESTS_maths_quaternion_tests[] = {
	{ "constructors", constructors_test26 },
	{ "swizzle", swizzle_test27 },
	{ "angle_axis", angle_axis_test28 },
	{ "euler", euler_test29 },
	{ "inverse", inverse_test30 },
};
TestSuite TESTS_maths_misc_tests[] = {
	{ "angle_between", angle_between_test31 },
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

#ifndef TESTS_TEST_H
#define TESTS_TEST_H

static void indexing_test0();
static void find_test1();
static void append_test2();
static void insert_test3();
static void set_test4();
extern void operators_test5();
extern void swizzle_test6();
extern void operators_test7();
extern void swizzle_test8();
extern void operators_test9();
extern void swizzle_test10();
extern void constructors_test11();
extern void swizzle_test12();
extern void angle_axis_test13();
extern void euler_test14();
extern void inverse_test15();
extern void angle_between_test16();

TestSuite TESTS_array_tests[] = {
	{ "indexing", indexing_test0 },
	{ "find", find_test1 },
};
TestSuite TESTS_dynamic_array_tests[] = {
	{ "append", append_test2 },
	{ "insert", insert_test3 },
	{ "set", set_test4 },
};
TestSuite TESTS_vector2_tests[] = {
	{ "operators", operators_test5 },
	{ "swizzle", swizzle_test6 },
};
TestSuite TESTS_vector3_tests[] = {
	{ "operators", operators_test7 },
	{ "swizzle", swizzle_test8 },
};
TestSuite TESTS_vector4_tests[] = {
	{ "operators", operators_test9 },
	{ "swizzle", swizzle_test10 },
};
TestSuite TESTS_maths_quaternion_tests[] = {
	{ "constructors", constructors_test11 },
	{ "swizzle", swizzle_test12 },
	{ "angle_axis", angle_axis_test13 },
	{ "euler", euler_test14 },
	{ "inverse", inverse_test15 },
};
TestSuite TESTS_maths_misc_tests[] = {
	{ "angle_between", angle_between_test16 },
};
TestSuite TESTS_tests[] = {
	{ "array", nullptr, TESTS_array_tests, sizeof(TESTS_array_tests)/sizeof(TESTS_array_tests[0]) },
	{ "dynamic_array", nullptr, TESTS_dynamic_array_tests, sizeof(TESTS_dynamic_array_tests)/sizeof(TESTS_dynamic_array_tests[0]) },
	{ "vector2", nullptr, TESTS_vector2_tests, sizeof(TESTS_vector2_tests)/sizeof(TESTS_vector2_tests[0]) },
	{ "vector3", nullptr, TESTS_vector3_tests, sizeof(TESTS_vector3_tests)/sizeof(TESTS_vector3_tests[0]) },
	{ "vector4", nullptr, TESTS_vector4_tests, sizeof(TESTS_vector4_tests)/sizeof(TESTS_vector4_tests[0]) },
	{ "maths_quaternion", nullptr, TESTS_maths_quaternion_tests, sizeof(TESTS_maths_quaternion_tests)/sizeof(TESTS_maths_quaternion_tests[0]) },
	{ "maths_misc", nullptr, TESTS_maths_misc_tests, sizeof(TESTS_maths_misc_tests)/sizeof(TESTS_maths_misc_tests[0]) },
};

#endif // TESTS_TEST_H

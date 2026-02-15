#ifndef ARRAY_TEST_H
#define ARRAY_TEST_H

extern void array__indexing();
extern void array__iterator();
extern void dynamic_array__copy_replaces_contents();
extern void dynamic_array__append();
extern void dynamic_array__insert();
extern void dynamic_array__set();
extern void dynamic_array__reserve();
extern void dynamic_array__resize();
extern void fixed_array__basic_construction();
extern void fixed_array__copy_operations();
extern void fixed_array__copy_invokes_copy_constructor_for_each_element();
extern void fixed_array__self_assign_avoids_copies();
extern void fixed_array__size_limits();
extern void fixed_array__data_pointer_integrity();
extern void array__pop();
extern void array__tail();
extern void array__create();
extern void array__remove();
extern void array__slice();
extern void array__find();
extern void array__swap();
extern void array__sort();

TestSuite ARRAY__array__tests[] = {
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

TestSuite ARRAY__dynamic_array__tests[] = {
	{ "copy_replaces_contents", dynamic_array__copy_replaces_contents },
	{ "append", dynamic_array__append },
	{ "insert", dynamic_array__insert },
	{ "set", dynamic_array__set },
	{ "reserve", dynamic_array__reserve },
	{ "resize", dynamic_array__resize },
};

TestSuite ARRAY__fixed_array__tests[] = {
	{ "basic_construction", fixed_array__basic_construction },
	{ "copy_operations", fixed_array__copy_operations },
	{ "copy_invokes_copy_constructor_for_each_element", fixed_array__copy_invokes_copy_constructor_for_each_element },
	{ "self_assign_avoids_copies", fixed_array__self_assign_avoids_copies },
	{ "size_limits", fixed_array__size_limits },
	{ "data_pointer_integrity", fixed_array__data_pointer_integrity },
};

TestSuite ARRAY__tests[] = {
	{ "array", nullptr, ARRAY__array__tests, sizeof(ARRAY__array__tests)/sizeof(ARRAY__array__tests[0]) },
	{ "dynamic_array", nullptr, ARRAY__dynamic_array__tests, sizeof(ARRAY__dynamic_array__tests)/sizeof(ARRAY__dynamic_array__tests[0]) },
	{ "fixed_array", nullptr, ARRAY__fixed_array__tests, sizeof(ARRAY__fixed_array__tests)/sizeof(ARRAY__fixed_array__tests[0]) },
};

#endif // ARRAY_TEST_H

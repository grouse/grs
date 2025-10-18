#ifndef MAP_TEST_H
#define MAP_TEST_H

static void dynamic_map__set_invokes_copy_constructor_for_key_and_value();
static void dynamic_map__growing_map_invokes_copy_constructors();
static void dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key();

TestSuite MAP__dynamic_map__tests[] = {
	{ "set_invokes_copy_constructor_for_key_and_value", dynamic_map__set_invokes_copy_constructor_for_key_and_value },
	{ "growing_map_invokes_copy_constructors", dynamic_map__growing_map_invokes_copy_constructors },
	{ "set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key", dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key },
};

TestSuite MAP__tests[] = {
	{ "dynamic_map", nullptr, MAP__dynamic_map__tests, sizeof(MAP__dynamic_map__tests)/sizeof(MAP__dynamic_map__tests[0]) },
};

#endif // MAP_TEST_H

#ifndef MEMORY_TEST_H
#define MEMORY_TEST_H

extern void memory__tl_linear__alloc_returns_nonnull();
extern void memory__tl_linear__alloc_zero_size_returns_nonnull();
extern void memory__tl_linear__alloc_respects_alignment();
extern void memory__tl_linear__sequential_allocs_dont_overlap();
extern void memory__tl_linear__free_is_noop();
extern void memory__tl_linear__realloc_zero_size_returns_nonnull();
extern void memory__tl_linear__realloc_last_alloc_is_inplace();
extern void memory__tl_linear__realloc_non_last_alloc_copies_data();
extern void memory__tl_linear__realloc_null_ptr_acts_as_alloc();
extern void memory__tl_linear__extend_zero_size_returns_nonnull();
extern void memory__tl_linear__extend_last_alloc_is_inplace();
extern void memory__tl_linear__extend_non_last_alloc_does_not_copy_data();
extern void memory__tl_linear__reset_to_start();
extern void memory__tl_linear__reset_to_restore_point();
extern void memory__tl_linear__alloc_beyond_capacity_panics();
extern void memory__tl_linear__get_allocator_info_reports_usage();
extern void memory__linear__get_allocator_info_reports_usage();
extern void memory__malloc__alloc_returns_nonnull();
extern void memory__malloc__alloc_zero_size_returns_nonnull();
extern void memory__malloc__alloc_respects_alignment();
extern void memory__malloc__free_null_is_safe();
extern void memory__malloc__realloc_preserves_data();
extern void memory__malloc__realloc_null_ptr_acts_as_alloc();
extern void memory__malloc__extend_is_fresh_alloc_no_copy();
extern void memory__malloc__reset_logs_error_and_returns_null();
extern void memory__malloc__get_allocator_info_reports_zero_usage();
extern void memory__malloc__get_allocator_info_stays_zero_after_allocations();
extern void memory__vm_freelist__get_allocator_info_reports_capacity();
extern void memory__vm_freelist__get_allocator_info_tracks_alloc_and_free();
extern void memory__scratch__alloc_returns_usable_memory();
extern void memory__scratch__release_restores_to_restore_point();
extern void memory__scratch__no_conflict_reuses_same_underlying_arena();
extern void memory__scratch__conflict_returns_different_arena();
extern void memory__scratch__non_scratch_conflict_still_returns_arena();
extern void memory__scratch__nested_caller_callee_use_different_arenas();
extern void memory__scratch__nested_inner_release_does_not_corrupt_outer();
extern void memory__scratch__nested_return_data_on_outer_survives_inner_release();
extern void memory__scratch__three_level_nesting_no_overlap();
extern void memory__scratch__four_level_nesting_no_overlap();
extern void memory__scratch__sarena_raii_restores_on_scope_exit();
extern void memory__scratch__alloc_before_and_after_inner_scope_both_survive();
extern void memory__scratch__sarena_nested_scopes_lifo_ordering();
extern void memory__scratch__multiple_allocs_accumulate_before_release();
extern void memory__arena__restore_resets_to_creation_point();
extern void memory__arena__get_info_size_is_constant();
extern void memory__arena__get_info_used_tracks_allocations();
extern void memory__buffer__write_and_read_back_typed();
extern void memory__buffer__offset_advances_correctly();
extern void memory__buffer__read_raw_bytes_returns_correct_pointer();
extern void memory__buffer__read_beyond_end_panics();
extern void memory__buffer__write_beyond_end_panics();
extern void memory__buffer__typed_read_beyond_end_panics();
extern void memory__buffer__typed_write_beyond_end_panics();
extern void memory__macros__alloc_t_constructs_typed_object();
extern void memory__macros__alloc_arr_allocates_typed_array();
extern void memory__macros__realloc_arr_preserves_existing_data();
extern void memory__macros__alloc_t_on_arena_constructs_object();
extern void memory__macros__alloc_t_runs_default_initialisers();
extern void memory__macros__alloc_t_default_init_can_be_overridden();
extern void memory__macros__alloc_t_on_arena_runs_default_initialisers();

TestSuite MEMORY__memory__tl_linear__tests[] = {
	{ "alloc_returns_nonnull", memory__tl_linear__alloc_returns_nonnull },
	{ "alloc_zero_size_returns_nonnull", memory__tl_linear__alloc_zero_size_returns_nonnull },
	{ "alloc_respects_alignment", memory__tl_linear__alloc_respects_alignment },
	{ "sequential_allocs_dont_overlap", memory__tl_linear__sequential_allocs_dont_overlap },
	{ "free_is_noop", memory__tl_linear__free_is_noop },
	{ "realloc_zero_size_returns_nonnull", memory__tl_linear__realloc_zero_size_returns_nonnull },
	{ "realloc_last_alloc_is_inplace", memory__tl_linear__realloc_last_alloc_is_inplace },
	{ "realloc_non_last_alloc_copies_data", memory__tl_linear__realloc_non_last_alloc_copies_data },
	{ "realloc_null_ptr_acts_as_alloc", memory__tl_linear__realloc_null_ptr_acts_as_alloc },
	{ "extend_zero_size_returns_nonnull", memory__tl_linear__extend_zero_size_returns_nonnull },
	{ "extend_last_alloc_is_inplace", memory__tl_linear__extend_last_alloc_is_inplace },
	{ "extend_non_last_alloc_does_not_copy_data", memory__tl_linear__extend_non_last_alloc_does_not_copy_data },
	{ "reset_to_start", memory__tl_linear__reset_to_start },
	{ "reset_to_restore_point", memory__tl_linear__reset_to_restore_point },
	{ "alloc_beyond_capacity_panics", memory__tl_linear__alloc_beyond_capacity_panics },
	{ "get_allocator_info_reports_usage", memory__tl_linear__get_allocator_info_reports_usage },
};

TestSuite MEMORY__memory__linear__tests[] = {
	{ "get_allocator_info_reports_usage", memory__linear__get_allocator_info_reports_usage },
};

TestSuite MEMORY__memory__malloc__tests[] = {
	{ "alloc_returns_nonnull", memory__malloc__alloc_returns_nonnull },
	{ "alloc_zero_size_returns_nonnull", memory__malloc__alloc_zero_size_returns_nonnull },
	{ "alloc_respects_alignment", memory__malloc__alloc_respects_alignment },
	{ "free_null_is_safe", memory__malloc__free_null_is_safe },
	{ "realloc_preserves_data", memory__malloc__realloc_preserves_data },
	{ "realloc_null_ptr_acts_as_alloc", memory__malloc__realloc_null_ptr_acts_as_alloc },
	{ "extend_is_fresh_alloc_no_copy", memory__malloc__extend_is_fresh_alloc_no_copy },
	{ "reset_logs_error_and_returns_null", memory__malloc__reset_logs_error_and_returns_null },
	{ "get_allocator_info_reports_zero_usage", memory__malloc__get_allocator_info_reports_zero_usage },
	{ "get_allocator_info_stays_zero_after_allocations", memory__malloc__get_allocator_info_stays_zero_after_allocations },
};

TestSuite MEMORY__memory__vm_freelist__tests[] = {
	{ "get_allocator_info_reports_capacity", memory__vm_freelist__get_allocator_info_reports_capacity },
	{ "get_allocator_info_tracks_alloc_and_free", memory__vm_freelist__get_allocator_info_tracks_alloc_and_free },
};

TestSuite MEMORY__memory__scratch__tests[] = {
	{ "alloc_returns_usable_memory", memory__scratch__alloc_returns_usable_memory },
	{ "release_restores_to_restore_point", memory__scratch__release_restores_to_restore_point },
	{ "no_conflict_reuses_same_underlying_arena", memory__scratch__no_conflict_reuses_same_underlying_arena },
	{ "conflict_returns_different_arena", memory__scratch__conflict_returns_different_arena },
	{ "non_scratch_conflict_still_returns_arena", memory__scratch__non_scratch_conflict_still_returns_arena },
	{ "nested_caller_callee_use_different_arenas", memory__scratch__nested_caller_callee_use_different_arenas },
	{ "nested_inner_release_does_not_corrupt_outer", memory__scratch__nested_inner_release_does_not_corrupt_outer },
	{ "nested_return_data_on_outer_survives_inner_release", memory__scratch__nested_return_data_on_outer_survives_inner_release },
	{ "three_level_nesting_no_overlap", memory__scratch__three_level_nesting_no_overlap },
	{ "four_level_nesting_no_overlap", memory__scratch__four_level_nesting_no_overlap },
	{ "sarena_raii_restores_on_scope_exit", memory__scratch__sarena_raii_restores_on_scope_exit },
	{ "alloc_before_and_after_inner_scope_both_survive", memory__scratch__alloc_before_and_after_inner_scope_both_survive },
	{ "sarena_nested_scopes_lifo_ordering", memory__scratch__sarena_nested_scopes_lifo_ordering },
	{ "multiple_allocs_accumulate_before_release", memory__scratch__multiple_allocs_accumulate_before_release },
};

TestSuite MEMORY__memory__arena__tests[] = {
	{ "restore_resets_to_creation_point", memory__arena__restore_resets_to_creation_point },
	{ "get_info_size_is_constant", memory__arena__get_info_size_is_constant },
	{ "get_info_used_tracks_allocations", memory__arena__get_info_used_tracks_allocations },
};

TestSuite MEMORY__memory__buffer__tests[] = {
	{ "write_and_read_back_typed", memory__buffer__write_and_read_back_typed },
	{ "offset_advances_correctly", memory__buffer__offset_advances_correctly },
	{ "read_raw_bytes_returns_correct_pointer", memory__buffer__read_raw_bytes_returns_correct_pointer },
	{ "read_beyond_end_panics", memory__buffer__read_beyond_end_panics },
	{ "write_beyond_end_panics", memory__buffer__write_beyond_end_panics },
	{ "typed_read_beyond_end_panics", memory__buffer__typed_read_beyond_end_panics },
	{ "typed_write_beyond_end_panics", memory__buffer__typed_write_beyond_end_panics },
};

TestSuite MEMORY__memory__macros__tests[] = {
	{ "alloc_t_constructs_typed_object", memory__macros__alloc_t_constructs_typed_object },
	{ "alloc_arr_allocates_typed_array", memory__macros__alloc_arr_allocates_typed_array },
	{ "realloc_arr_preserves_existing_data", memory__macros__realloc_arr_preserves_existing_data },
	{ "alloc_t_on_arena_constructs_object", memory__macros__alloc_t_on_arena_constructs_object },
	{ "alloc_t_runs_default_initialisers", memory__macros__alloc_t_runs_default_initialisers },
	{ "alloc_t_default_init_can_be_overridden", memory__macros__alloc_t_default_init_can_be_overridden },
	{ "alloc_t_on_arena_runs_default_initialisers", memory__macros__alloc_t_on_arena_runs_default_initialisers },
};

TestSuite MEMORY__tests[] = {
	{ "memory/arena", nullptr, MEMORY__memory__arena__tests, sizeof(MEMORY__memory__arena__tests)/sizeof(MEMORY__memory__arena__tests[0]) },
	{ "memory/buffer", nullptr, MEMORY__memory__buffer__tests, sizeof(MEMORY__memory__buffer__tests)/sizeof(MEMORY__memory__buffer__tests[0]) },
	{ "memory/linear", nullptr, MEMORY__memory__linear__tests, sizeof(MEMORY__memory__linear__tests)/sizeof(MEMORY__memory__linear__tests[0]) },
	{ "memory/macros", nullptr, MEMORY__memory__macros__tests, sizeof(MEMORY__memory__macros__tests)/sizeof(MEMORY__memory__macros__tests[0]) },
	{ "memory/malloc", nullptr, MEMORY__memory__malloc__tests, sizeof(MEMORY__memory__malloc__tests)/sizeof(MEMORY__memory__malloc__tests[0]) },
	{ "memory/scratch", nullptr, MEMORY__memory__scratch__tests, sizeof(MEMORY__memory__scratch__tests)/sizeof(MEMORY__memory__scratch__tests[0]) },
	{ "memory/tl_linear", nullptr, MEMORY__memory__tl_linear__tests, sizeof(MEMORY__memory__tl_linear__tests)/sizeof(MEMORY__memory__tl_linear__tests[0]) },
	{ "memory/vm_freelist", nullptr, MEMORY__memory__vm_freelist__tests, sizeof(MEMORY__memory__vm_freelist__tests)/sizeof(MEMORY__memory__vm_freelist__tests[0]) },
};

#endif // MEMORY_TEST_H

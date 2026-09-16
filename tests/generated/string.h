#ifndef STRING_TESTS_H
#define STRING_TESTS_H

extern void string_builder__append_stringf_preserves_last_character_at_block_boundary();
extern void string_builder__append_stringf_handles_full_block();
extern void string_builder__append_char_appends_one_byte();
extern void string_builder__sz_string_copies_all_blocks();
extern void string__u64_from_string_accepts_decimal_and_hex();
extern void string__u64_from_string_rejects_invalid_and_overflow_values();

TestSuite STRING__string_builder__tests[] = {
	{ "append_stringf_preserves_last_character_at_block_boundary", string_builder__append_stringf_preserves_last_character_at_block_boundary },
	{ "append_stringf_handles_full_block", string_builder__append_stringf_handles_full_block },
	{ "append_char_appends_one_byte", string_builder__append_char_appends_one_byte },
	{ "sz_string_copies_all_blocks", string_builder__sz_string_copies_all_blocks },
};

TestSuite STRING__string__tests[] = {
	{ "u64_from_string_accepts_decimal_and_hex", string__u64_from_string_accepts_decimal_and_hex },
	{ "u64_from_string_rejects_invalid_and_overflow_values", string__u64_from_string_rejects_invalid_and_overflow_values },
};

TestSuite STRING__tests[] = {
	{ "string", nullptr, STRING__string__tests, sizeof(STRING__string__tests)/sizeof(STRING__string__tests[0]) },
	{ "string_builder", nullptr, STRING__string_builder__tests, sizeof(STRING__string_builder__tests)/sizeof(STRING__string_builder__tests[0]) },
};

#endif // STRING_TESTS_H

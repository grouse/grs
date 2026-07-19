#ifndef STRING_TEST_H
#define STRING_TEST_H

extern void string_builder__append_stringf_preserves_last_character_at_block_boundary();
extern void string_builder__append_stringf_handles_full_block();
extern void string_builder__append_char_appends_one_byte();
extern void string_builder__sz_string_copies_all_blocks();

TestSuite STRING__string_builder__tests[] = {
	{ "append_stringf_preserves_last_character_at_block_boundary", string_builder__append_stringf_preserves_last_character_at_block_boundary },
	{ "append_stringf_handles_full_block", string_builder__append_stringf_handles_full_block },
	{ "append_char_appends_one_byte", string_builder__append_char_appends_one_byte },
	{ "sz_string_copies_all_blocks", string_builder__sz_string_copies_all_blocks },
};

TestSuite STRING__tests[] = {
	{ "string_builder", nullptr, STRING__string_builder__tests, sizeof(STRING__string_builder__tests)/sizeof(STRING__string_builder__tests[0]) },
};

#endif // STRING_TEST_H

#include "core/string.h"
#include "core/test.h"

TEST_PROC(string_builder__append_stringf_preserves_last_character_at_block_boundary)
{
    char prefix[4090];
    memset(prefix, 'x', sizeof prefix);

    StringBuilder sb{};
    append_string(&sb, String{ prefix, i32(sizeof prefix) });
    append_stringf(&sb, "tail\n");

    String result = create_string(&sb, mem_dynamic);
    ASSERT(result.length == i32(sizeof prefix) + 5);
    ASSERT(memcmp(result.data + sizeof prefix, "tail\n", 5) == 0);

    FREE(mem_dynamic, result.data);
}

TEST_PROC(string_builder__append_stringf_handles_full_block)
{
    char prefix[4096];
    memset(prefix, 'x', sizeof prefix);

    StringBuilder sb{};
    append_string(&sb, String{ prefix, i32(sizeof prefix) });
    append_stringf(&sb, "tail\n");

    String result = create_string(&sb, mem_dynamic);
    ASSERT(result.length == i32(sizeof prefix) + 5);
    ASSERT(memcmp(result.data + sizeof prefix, "tail\n", 5) == 0);

    FREE(mem_dynamic, result.data);
    FREE(mem_dynamic, sb.head.next);
}

TEST_PROC(string_builder__append_char_appends_one_byte)
{
    StringBuilder sb{};
    append_char(&sb, '\n');

    String result = create_string(&sb, mem_dynamic);
    ASSERT(result.length == 1);
    ASSERT(result[0] == '\n');

    FREE(mem_dynamic, result.data);
}

TEST_PROC(string_builder__sz_string_copies_all_blocks)
{
    char prefix[4096];
    memset(prefix, 'x', sizeof prefix);

    StringBuilder sb{};
    append_string(&sb, String{ prefix, i32(sizeof prefix) });
    append_string(&sb, string("tail"));

    char *result = sz_string(&sb, mem_dynamic);
    ASSERT(strlen(result) == sizeof prefix + 4);
    ASSERT(memcmp(result, prefix, sizeof prefix) == 0);
    ASSERT(memcmp(result + sizeof prefix, "tail", 4) == 0);

    FREE(mem_dynamic, result);
    FREE(mem_dynamic, sb.head.next);
}

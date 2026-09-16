#include "core/lexer.h"
#include "core/test.h"

TEST_PROC(lexer__hex_integer_stops_before_punctuation)
{
    String input = "0x0123456789abcdef;";
    Lexer lexer{ input, "lexer__hex_integer_stops_before_punctuation" };

    u64 value = 0;
    ASSERT(parse_u64(&lexer, &value));
    ASSERT(value == 0x0123456789abcdefull);
    ASSERT(require_next_token(&lexer, ';'));
}

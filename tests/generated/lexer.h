#ifndef LEXER_TESTS_H
#define LEXER_TESTS_H

extern void lexer__hex_integer_stops_before_punctuation();

TestSuite LEXER__lexer__tests[] = {
	{ "hex_integer_stops_before_punctuation", lexer__hex_integer_stops_before_punctuation },
};

TestSuite LEXER__tests[] = {
	{ "lexer", nullptr, LEXER__lexer__tests, sizeof(LEXER__lexer__tests)/sizeof(LEXER__lexer__tests[0]) },
};

#endif // LEXER_TESTS_H

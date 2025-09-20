#ifndef LEXER_PUBLIC_H
#define LEXER_PUBLIC_H

extern Token next_token(Lexer *lexer, u32 flags);
extern Token next_nth_token(Lexer *lexer, i32 n, u32 flags);
extern Token peek_token(Lexer *lexer, u32 flags);
extern Token peek_nth_token(Lexer *lexer, i32 n, u32 flags);
extern Token eat_until(Lexer *lexer, TokenType terminator, u32 flags);
extern bool require_next_token(Lexer *lexer, TokenType type, Token *t = nullptr);
extern bool require_next_token(Lexer *lexer, char c, Token *t = nullptr);
extern bool require_next_identifier(Lexer *lexer, String identifier, Token *t = nullptr);
extern bool optional_token(Lexer *lexer, TokenType type, Token *t = nullptr);
extern bool optional_token(Lexer *lexer, char c, Token *t = nullptr);
extern bool is_identifier(Token t, String str);
extern bool parse_version_decl(Lexer *lexer, i32 *version_out, i32 max_version);
extern bool parse_float(Lexer *lexer, f32 *value);
extern bool parse_floatn(Lexer *lexer, i32 n, f32 *value);
extern bool parse_int(Lexer *lexer, i32 *value);
extern bool parse_string(Lexer *lexer, String *str);
extern bool parse_string(Lexer *lexer, String *str, Allocator mem);
extern bool parse_bool(Lexer *lexer, bool *value);

#endif // LEXER_PUBLIC_H

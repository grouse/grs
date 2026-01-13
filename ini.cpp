#include "ini.h"

#include "lexer.h"
#include "file.h"

IniSerializer ini_writer(Allocator mem) EXPORT
{
    return {
        .mode = INI_WRITE,
        .out.alloc = mem,
    };
}

IniSerializer ini_reader(u8 *data, i32 size) EXPORT
{
    IniSerializer ini = { .mode = INI_READ };
    ini.lexer = Lexer{ data, size, "ini" };
    ini.prev_t = {};
    next_token(&ini.lexer);
    return ini;
}

IniSerializer ini_read_file(String path, Allocator mem) EXPORT
{
    FileInfo f = read_file(path, mem);

    IniSerializer ini = { .mode = INI_READ };
    ini.lexer = Lexer{ f.data, f.size, path };
    next_token(&ini.lexer);
    return ini;
}

bool ini_section_begin(IniSerializer *ini, String name) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        array_add(&ini->sections, name);

        append_stringf(&ini->out, "[%.*s", STRFMT(ini->sections[0]));
        for (String section : slice(ini->sections, 1)) {
            append_stringf(&ini->out, ".%.*s", STRFMT(section));
        }
        append_string(&ini->out, "]\n");
        break;
    case INI_READ:
        if (ini->lexer.t == '[') {
            i32 parent_depth = 0;
            while (ini->lexer.t != ']' && ini->lexer.t != TOKEN_EOF) {
                if (!require_next_token(&ini->lexer, TOKEN_IDENTIFIER, &ini->lexer.t)) return false;
                if (parent_depth >= ini->sections.count || ini->sections[parent_depth] != ini->lexer.t.str) {
                    ini->sections.count = parent_depth;
                    array_add(&ini->sections, ini->lexer.t.str);
                } else parent_depth++;
                next_token(&ini->lexer);
            }

            next_token(&ini->lexer);
        }

        if (ini->sections.count == 0) return false;
        if (*array_tail(ini->sections) != name) return false;
        break;
    }

    return true;
}

bool ini_section_end(IniSerializer *ini) EXPORT
{
    if (ini->sections.count > 0) {
        array_pop(&ini->sections);
        return true;
    }

    return false;
}

bool ini_value(IniSerializer *ini, String name, bool *value) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        append_stringf(&ini->out, "%.*s = %s\n", STRFMT(name), *value ? "true" : "false");
        return true;
    case INI_READ:
        if (is_identifier(ini->lexer.t, name)) {
            if (!require_next_token(&ini->lexer, '=', &ini->lexer.t)) return false;
            if (!parse_bool(&ini->lexer, value)) return false;
            next_token(&ini->lexer);
            return true;
        }
        break;
    }

    return false;
}

bool ini_value(
    IniSerializer *ini,
    String name,
    i32 *value, int count /*= 1*/) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        append_stringf(&ini->out, "%.*s = %d", STRFMT(name), *value);
        for (i32 i = 1; i < count; i++) append_stringf(&ini->out, ", %d", value[i]);
        append_string(&ini->out, "\n");
        return true;
    case INI_READ:
        if (is_identifier(ini->lexer.t, name)) {
            if (!require_next_token(&ini->lexer, '=', &ini->lexer.t)) return false;
            for (i32 i = 0; i < count; i++) {
                if (!parse_int(&ini->lexer, &value[i])) return false;
                if (i == count-1 || !optional_token(&ini->lexer, ',', &ini->lexer.t)) break;
            }

            next_token(&ini->lexer);
            return true;
        }
        break;
    }

    return false;
}

bool ini_value(
    IniSerializer *ini,
    String name,
    f32 *value, i32 count /*= 1*/) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        append_stringf(&ini->out, "%.*s = %f", STRFMT(name), *value);
        for (i32 i = 1; i < count; i++) append_stringf(&ini->out, ", %f", value[i]);
        append_string(&ini->out, "\n");
        return true;
    case INI_READ:
        if (is_identifier(ini->lexer.t, name)) {
            if (!require_next_token(&ini->lexer, '=', &ini->lexer.t)) return false;
            for (i32 i = 0; i < count; i++) {
                if (!parse_float(&ini->lexer, &value[i])) return false;
                if (i == count-1 || !optional_token(&ini->lexer, ',', &ini->lexer.t)) break;
            }

            next_token(&ini->lexer);
            return true;
        }
    }

    return false;
}

bool ini_value(IniSerializer *ini, String name, String *value, Allocator mem) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        append_stringf(&ini->out, "%.*s = \"%.*s\"\n", STRFMT(name), STRFMT(*value));
        return true;
    case INI_READ:
        if (is_identifier(ini->lexer.t, name)) {
            if (!require_next_token(&ini->lexer, '=', &ini->lexer.t)) return false;
            String str;
            if (!parse_string(&ini->lexer, &str)) return false;
            *value = duplicate_string(str, mem);

            next_token(&ini->lexer);
            return true;
        }
        break;
    }
    return false;
}


bool ini_in_section(IniSerializer *ini) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE: return false;
    case INI_READ:
        if (ini->lexer.t == '[') return false;
        return ini->lexer.t != TOKEN_EOF;
    }
}

bool ini_next(IniSerializer *ini) EXPORT
{
    switch (ini->mode) {
    case INI_WRITE:
        return false;
    case INI_READ:
        if (ini->lexer.t == TOKEN_EOF) return false;

        if (ini->prev_t == ini->lexer.t) {
            LOG_INFO("unhandled ini field: %.*s, skipping", STRFMT(ini->lexer.t.str));
            eat_until(&ini->lexer, TOKEN_NEWLINE, LEXER_NEWLINE);
            next_token(&ini->lexer, LEXER_NEWLINE);
        }

        ini->prev_t = ini->lexer.t;
        return ini->lexer.t != TOKEN_EOF;
    }
}

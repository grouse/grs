#ifndef INI_H
#define INI_H

#include "string.h"
#include "lexer.h"
#include "array.h"

enum IniMode {
    INI_READ,
    INI_WRITE,
};

struct IniSerializer {
    FixedArray<String, 4> sections;

    IniMode mode;

    union {
        struct { StringBuilder out; };
        struct {
            Lexer lexer;
            Token prev_t;
        };
    };
};

#define ini_section(ini, ...)\
    for (bool VAR(ini_section) = ini_section_begin(ini ARGS(__VA_ARGS__));\
         VAR(ini_section);\
         VAR(ini_section) = ini_in_section(ini) || (ini_section_end(ini), false))

#define ini_bitfield_value(ini, name, bit)\
    do {\
        bool VAR(value) = bit;\
        ini_value(ini, name, &VAR(value));\
        bit = VAR(value);\
    } while(0)


#include "generated/ini.h"

#endif // INI_H

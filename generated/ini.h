#ifndef INI_PUBLIC_H
#define INI_PUBLIC_H

extern IniSerializer ini_writer(Allocator mem);
extern IniSerializer ini_reader(u8 *data, i32 size);
extern IniSerializer ini_read_file(String path, Allocator mem);
extern bool ini_section_begin(IniSerializer *ini, String name);
extern bool ini_section_end(IniSerializer *ini);
extern bool ini_value(IniSerializer *ini, String name, bool *value);
extern bool ini_value(IniSerializer *ini, String name, i32 *value, int count = 1);
extern bool ini_value(IniSerializer *ini, String name, f32 *value, i32 count = 1);
extern bool ini_value(IniSerializer *ini, String name, String *value, Allocator mem);
extern bool ini_in_section(IniSerializer *ini);
extern bool ini_next(IniSerializer *ini);

#endif // INI_PUBLIC_H

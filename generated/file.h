#ifndef FILE_GENERATED_H
#define FILE_GENERATED_H

extern FileInfo read_file(String path, Allocator mem, i32 retry_count);
extern void list_files(DynamicArray<String> *dst, String dir, Array<String> extensions, Allocator mem, u32 flags);
extern void list_folders(DynamicArray<String> *dst, String dir, Allocator mem, u32 flags);
extern void create_filewatch(String folder, DynamicArray<FileEvent> *events, Mutex *events_mutex);
extern String absolute_path(String relative, Allocator mem);
extern FileHandle open_file(String path, u32 mode);
extern void write_file(FileHandle handle, const void *data, i32 bytes);
extern void read_file(FileHandle handle, void *data, i32 bytes);
extern void close_file(FileHandle handle);
extern void write_file(String path, const void *data, i32 bytes);
extern void write_file(String path, StringBuilder *sb);
extern bool is_directory(String path);
extern bool file_exists_sz(const char *path);
extern bool file_exists(String path);
extern void remove_file(String path);
extern void remove_files(String path, u32 flags);
extern String get_exe_folder(Allocator mem);
extern String get_working_dir(Allocator mem);
extern void set_working_dir(String path);
extern u64 file_modified_timestamp(String path);
extern String local_user_log_dir(Allocator mem);
extern String uri_from_path(String path, Allocator mem);

#endif // FILE_GENERATED_H

#ifdef FILE_GENERATED_IMPL
#define FILE_INTERNAL
#endif

#ifndef FILE_H
#define FILE_H

#ifdef _WIN32
#include "win32_lite.h"
using FileHandle = HANDLE;
#define FILE_HANDLE_INVALID ((HANDLE)(LONG_PTR)-1)
#else
struct FileHandle_;
using FileHandle = FileHandle_*;
#define FILE_HANDLE_INVALID (nullptr)
#endif

#include "array.h"
#include "string.h"
#include "thread.h"

struct FileInfo {
    u8 *data;
    i32 size;
};

enum FileOpenMode {
    FILE_OPEN_READ     = 1 << 0,
    FILE_OPEN_WRITE    = 1 << 1,
    FILE_OPEN_CREATE   = 1 << 2,
    FILE_OPEN_TRUNCATE = 1 << 3,

    FILE_OPEN_RW = FILE_OPEN_READ | FILE_OPEN_WRITE
};

enum ListFileFlags : u32 {
    FILE_LIST_RECURSIVE = 1 << 0,
    FILE_LIST_ABSOLUTE  = 1 << 1,
};

enum FileEventType {
    FE_UNKNOWN = 0,
    FE_MODIFY,
    FE_CREATE,
    FE_DELETE,
};

struct FileEvent {
    FileEventType type;
    String path;
};

FileInfo read_file(String path, Allocator mem, i32 retry_count = 0);

void list_files(DynamicArray<String> *dst, String dir, Array<String> extensions, Allocator mem, u32 flags = 0);

inline void list_files(DynamicArray<String> *dst, String dir, std::initializer_list<String> extensions, Allocator mem, u32 flags = 0)
{
    list_files(dst, dir, Array<String>{ (String*)extensions.begin(), (i32)extensions.size() }, mem, flags);
}

inline void list_files(DynamicArray<String> *dst, String dir, String ext, Allocator mem, u32 flags = 0)
{
    list_files(dst, dir, { &ext, 1 }, mem, flags);
}

inline void list_files(DynamicArray<String> *dst, String dir, Allocator mem, u32 flags = 0)
{
    list_files(dst, dir, {}, mem, flags);
}

inline DynamicArray<String> list_files(String dir, Allocator mem, u32 flags = 0)
{
    DynamicArray<String> files{ .alloc = mem };
    list_files(&files, dir, {}, mem, flags);
    return files;
}

void list_folders(DynamicArray<String> *dst, String dir, Allocator mem, u32 flags = 0);
inline DynamicArray<String> list_folders(String dir, Allocator mem, u32 flags = 0)
{
	DynamicArray<String> dirs{ .alloc = mem };
	list_folders(&dirs, dir, mem, flags);
	return dirs;
}

#include "generated/file.h"

void create_filewatch(String folder, DynamicArray<FileEvent> *events, Mutex *events_mutex);

String absolute_path(String relative, Allocator mem);

FileHandle open_file(String path, u32 mode = FILE_OPEN_RW);
void write_file(FileHandle handle, const void *data, i32 bytes);
void read_file(FileHandle handle, void *data, i32 bytes);
void close_file(FileHandle handle);

void write_file(String path, const void *data, i32 bytes);
void write_file(String path, StringBuilder *sb);

bool is_directory(String path);
bool file_exists_sz(const char *path);
bool file_exists(String path);

void remove_file(String path);
void remove_files(String path, u32 flags = 0);

String get_exe_folder(Allocator mem);
String get_working_dir(Allocator mem);
void set_working_dir(String path);

u64 file_modified_timestamp(String path);

#endif //FILE_H

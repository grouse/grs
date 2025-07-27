#ifndef MEMORY_H
#define MEMORY_H

#include <new>
#include "core.h"

extern "C" CRTIMP void* malloc(size_t size) NOTHROW;
extern "C" CRTIMP void* realloc(void* ptr, size_t size) NOTHROW;
extern "C" CRTIMP void free(void* ptr) NOTHROW;

extern "C" void* memcpy(void *destination, const void *source, size_t num) NOTHROW;
extern "C" void* memset(void *ptr, int value, size_t num) NOTHROW;
extern "C" int memcmp(const void *ptr1, const void *ptr2, size_t num) NOTHROW;
extern "C" void* memmove(void *destination, const void *source, size_t num) NOTHROW;

#define KiB (1024)
#define MiB (1024*1024)
#define GiB (1024LL*1024LL*1024LL)

#define M_SCRATCH_ARENAS 4
#define M_DEFAULT_ALIGN 16

enum M_Proc { M_ALLOC, M_FREE, M_EXTEND, M_REALLOC, M_RESET, };
typedef void* allocate_t(void *state, M_Proc cmd, const void *old_ptr, i64 old_size, i64 size, u8 alignment);

struct Allocator {
    void *state;
    allocate_t *proc;

    explicit operator bool() { return proc; }
    bool operator==(Allocator other) { return state == other.state && proc == other.proc; }
};

extern Allocator mem_dynamic;

#define ALLOCATOR(alloc) ((Allocator)(alloc))
#define ALLOC_STATE(alloc) (((Allocator)(alloc)).state)
#define ALLOC_PROC(alloc, ...) (((Allocator)(alloc)).proc(((Allocator)(alloc)).state, __VA_ARGS__))

#define ALLOC(alloc, size)               ALLOC_PROC(alloc, M_ALLOC, nullptr, 0, size,      M_DEFAULT_ALIGN)
#define ALLOC_A(alloc, size, align)      ALLOC_PROC(alloc, M_ALLOC, nullptr, 0, size,      align)
#define ALLOC_T(alloc, T)           new (ALLOC_PROC(alloc, M_ALLOC, nullptr, 0, sizeof(T), alignof(T))) T

#define FREE(alloc, ptr)                 ALLOC_PROC(alloc, M_FREE,    ptr,     0,     0,    0)

#define REALLOC(alloc, ptr, osize, size)          ALLOC_PROC(alloc, M_REALLOC, ptr,     osize, size, M_DEFAULT_ALIGN)
#define REALLOC_A(alloc, ptr, osize, size, align) ALLOC_PROC(alloc, M_REALLOC, ptr,     osize, size, align)

#define RESET_ALLOC(alloc)        ALLOC_PROC(alloc, M_RESET, nullptr, 0,     0,    0)
#define RESTORE_ALLOC(alloc, ptr) ALLOC_PROC(alloc, M_RESET, ptr,     0,     0,    0)


#define ALLOC_ARR(alloc, T, count)\
    (T*)ALLOC_PROC(alloc, M_ALLOC, nullptr, 0, sizeof(T)*(count), alignof(T))
#define REALLOC_ARR(alloc, T, old_ptr, old_count, new_count)\
    (T*)ALLOC_PROC(alloc, M_REALLOC, old_ptr, sizeof(T)*(old_count), sizeof(T)*(new_count), alignof(T))
#define REALLOC_ARR_A(alloc, T, old_ptr, old_count, new_count, alignment)\
    (T*)ALLOC_PROC(alloc, M_REALLOC, old_ptr, sizeof(T)*(old_count), sizeof(T)*(new_count), alignment)

#define EXTEND_ARR(alloc, T, old_ptr, old_count, new_count)\
    (T*)ALLOC_PROC(alloc, M_EXTEND, old_ptr, sizeof(T)*(old_count), sizeof(T)*(new_count), alignof(T))
#define EXTEND_ARR_A(alloc, T, old_ptr, old_count, new_count, alignment)\
    (T*)ALLOC_PROC(alloc, M_EXTEND, old_ptr, sizeof(T)*(old_count), sizeof(T)*(new_count), alignment)

void init_default_allocators();

i32 get_page_size();

void* virtual_reserve(i64 size);
void* virtual_commit(void *addr, i64 size);

Allocator linear_allocator(i64 size);
Allocator malloc_allocator();

Allocator tl_linear_allocator(i64 size);

Allocator vm_freelist_allocator(i64 max_size);


struct MArena : Allocator {
    void *restore_point;
};

MArena tl_scratch_arena(Allocator conflict = {});
void release_arena(MArena *arena);
void restore_arena(MArena *arena);


struct SArena {
    MArena arena;
    SArena(MArena &&arena) : arena(arena) {}
    SArena &operator=(MArena &&arena) { this->arena = arena; return *this; }

    SArena(const MArena &arena) = delete;
    SArena(const SArena &) = delete;
    SArena& operator=(const SArena &) = delete;

    ~SArena() { release_arena(&arena); }

    MArena* operator->() { return &arena; }
    MArena& operator*() { return arena; }
    operator Allocator() { return arena; }
};

struct Mutex;

// TODO(jesper): I think I want a thread-local version of this that doesn't do
// any thread safe guarding
// It also shouldn't be in the public memory.h, just here for convenience atm because win32_memory and linux_memory shares a bunch of code. Probably there should be a vm_memory or something
struct VMFreeListState {
    struct Block {
        Block *next;
        Block *prev;
        i64 size;
    };

    u8 *mem;
    i64 reserved;
    i64 committed;

    Block *free_block;
    Mutex *mutex;

    i32 page_size;
};

struct MemoryBuffer {
    u8 *data;
    i32 size;
    i32 offset;
};

inline void* read_memory(MemoryBuffer *buf, i32 size)
{
    PANIC_IF(buf->offset + size > buf->size, "reading beyond end of buffer");
    void *ptr = (buf->data + buf->offset);
    buf->offset += size;
    return ptr;
}

inline void write_memory(MemoryBuffer *buf, const void *data, i32 size)
{
    PANIC_IF(buf->offset + size > buf->size, "reading beyond end of buffer");
    memcpy(buf->data+buf->offset, data, size);
    buf->offset += size;
}


template<typename T>
inline T read_memory(MemoryBuffer *buf)
{
    PANIC_IF(buf->offset + (i32)sizeof(T) > buf->size, "reading beyond end of buffer");
    T *val = (T*)(buf->data + buf->offset);
    buf->offset += sizeof(T);
    return *val;
}

template<typename T>
inline void write_memory(MemoryBuffer *buf, T val)
{
    PANIC_IF(buf->offset + (i32)sizeof(T) > buf->size, "reading beyond end of buffer");
    memcpy(buf->data+buf->offset, &val, sizeof(T));
    buf->offset += sizeof(T);
}

#endif // MEMORY_H

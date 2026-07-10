#ifndef MEMORY_GENERATED_H
#define MEMORY_GENERATED_H

extern void init_default_allocators();
extern i32 get_page_size();
extern void *virtual_reserve(i64 size);
extern void *virtual_commit(void *addr, i64 size);
extern Allocator linear_allocator(i64 size);
extern Allocator malloc_allocator();
extern Allocator tl_linear_allocator(i64 size);
extern Allocator vm_freelist_allocator(i64 max_size);
extern MArena tl_arena(i32 initial_size);
extern MArena tl_scratch_arena(Allocator conflict);
extern void release_arena(MArena *arena);
extern void restore_arena(MArena *arena);
extern AllocatorInfo get_allocator_info(Allocator alloc);
extern i32 tl_scratch_idx(MArena arena);
extern void *align_ptr(void *ptr, u8 alignment, u8 header_size);
extern void init_default_allocators();
extern MArena tl_scratch_arena(Allocator conflict);
extern MArena tl_arena(i32 initial_size);
extern AllocatorInfo get_allocator_info(Allocator alloc);
extern void restore_arena(MArena *arena);
extern void release_arena(MArena *arena);
extern void *align_ptr(void *ptr, u8 alignment, u8 header_size);
extern void *tl_linear_alloc(void *v_state, M_Proc cmd, const void *old_ptr, i64 old_size, i64 size, u8 alignment);
extern void *linear_alloc(void *v_state, M_Proc cmd, const void *old_ptr, i64 old_size, i64 size, u8 alignment);
extern void *malloc_alloc(void *v_state, M_Proc cmd, const void *old_ptr, i64 old_size, i64 size, u8 alignment);
extern Allocator tl_linear_allocator(i64 size);
extern Allocator linear_allocator(i64 size);
extern Allocator malloc_allocator();
extern void *vm_freelist_alloc(void *v_state, M_Proc cmd, const void *old_ptr, i64 old_size, i64 size, u8 alignment);

#endif // MEMORY_GENERATED_H

#ifdef MEMORY_GENERATED_IMPL
#define MEMORY_INTERNAL
#endif

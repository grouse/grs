
#ifdef MEMORY_GENERATED_IMPL
#define MEMORY_INTERNAL
#endif

#if defined(MEMORY_INTERNAL) && !defined(MEMORY_INTERNAL_ONCE)
#define MEMORY_INTERNAL_ONCE

static bool is_aligned(void *ptr, u8 alignment);
static bool is_overlapping(void *a, i64 a_size, void *b, i64 b_size);
static i64 vm_freelist_exact_page_request_size(VMFreeListState *state, u8 alignment);
static i64 vm_freelist_near_exact_page_request_size(VMFreeListState *state, u8 alignment);

#endif

#include "core/memory.h"
#include "core/test.h"

static bool is_aligned(void *ptr, u8 alignment)
{
    return ((size_t)ptr & (size_t)(alignment-1)) == 0;
}

static bool is_overlapping(void *a, i64 a_size, void *b, i64 b_size)
{
    u8 *a_begin = (u8*)a, *a_end = a_begin + a_size;
    u8 *b_begin = (u8*)b, *b_end = b_begin + b_size;
    return a_end > b_begin && b_end > a_begin;
}

struct VMFreeListHeaderProbe {
    u64 total_size;
    u8 offset;
    u8 alignment;
};

static i64 vm_freelist_exact_page_request_size(VMFreeListState *state, u8 alignment = M_DEFAULT_ALIGN)
{
    return state->page_size - alignment - (i64)sizeof(VMFreeListHeaderProbe) + 1;
}

static i64 vm_freelist_near_exact_page_request_size(VMFreeListState *state, u8 alignment = M_DEFAULT_ALIGN)
{
    return vm_freelist_exact_page_request_size(state, alignment) - ((i64)sizeof(VMFreeListState::Block) - 1);
}

TEST_PROC(memory__tl_linear__alloc_returns_nonnull)
{
    Allocator a = tl_linear_allocator(4096);

    void *p = ALLOC(a, 16);
    ASSERT(p != nullptr);
}

TEST_PROC(memory__tl_linear__alloc_zero_size_returns_nonnull)
{
    Allocator a = tl_linear_allocator(4096);

    // Zero-size requests still need a usable pointer.
    void *p = ALLOC(a, 0);
    ASSERT(p != nullptr);

    // Arena should still be usable after a zero-size alloc.
    void *q = ALLOC(a, 16);
    ASSERT(q != nullptr);
}

TEST_PROC(memory__tl_linear__alloc_respects_alignment)
{
    Allocator a = tl_linear_allocator(4096);

    // Allocate a 1-byte item first to misalign the cursor, then check that
    // subsequent allocations still satisfy their alignment requirement.
    ALLOC_A(a, 1, 1);

    void *p1 = ALLOC_A(a, 4,  4);
    void *p2 = ALLOC_A(a, 8,  8);
    void *p3 = ALLOC_A(a, 16, 16);
    void *p4 = ALLOC_A(a, 4,  32);
    void *p5 = ALLOC_A(a, 4,  64);

    ASSERT(is_aligned(p1, 4));
    ASSERT(is_aligned(p2, 8));
    ASSERT(is_aligned(p3, 16));
    ASSERT(is_aligned(p4, 32));
    ASSERT(is_aligned(p5, 64));
}

TEST_PROC(memory__tl_linear__sequential_allocs_dont_overlap)
{
    Allocator a = tl_linear_allocator(4096);

    u8 *p0 = (u8*)ALLOC_A(a, 64, 1);
    memset(p0, 0xCA, 64);

    u8 *p1 = (u8*)ALLOC_A(a, 64, 1);
    memset(p1, 0xCB, 64);

    u8 *p2 = (u8*)ALLOC_A(a, 64, 1);
    memset(p2, 0xCC, 64);

    ASSERT(p1 >= p0 + 64);
    ASSERT(p2 >= p1 + 64);

    for (i32 i = 0; i < 64; i++) ASSERT(p0[i] == 0xCA);
    for (i32 i = 0; i < 64; i++) ASSERT(p1[i] == 0xCB);
}

TEST_PROC(memory__tl_linear__free_is_noop)
{
    Allocator a = tl_linear_allocator(4096);

    void *p = ALLOC(a, 64);
    ASSERT(p != nullptr);

    // FREE on a linear allocator is documented to be a no-op; must not crash
    void *ret = FREE(a, p);
    ASSERT(ret == nullptr);

    void *q = ALLOC(a, 64);
    ASSERT(q != nullptr);
    ASSERT(q != p);
}

TEST_PROC(memory__tl_linear__realloc_zero_size_returns_nonnull)
{
    Allocator a = tl_linear_allocator(4096);

    void *p = ALLOC(a, 64);
    ASSERT(p != nullptr);

    void *r = REALLOC(a, p, 64, 0);
    ASSERT(r != nullptr);
}

TEST_PROC(memory__tl_linear__realloc_last_alloc_is_inplace)
{
    Allocator a = tl_linear_allocator(4096);

    u8 *p = (u8*)ALLOC_A(a, 64, 1);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    u8 *r = (u8*)REALLOC(a, p, 64, 128);
    ASSERT(r == p);
    for (i32 i = 0; i < 64; i++) ASSERT(r[i] == (u8)i);
}

TEST_PROC(memory__tl_linear__realloc_non_last_alloc_copies_data)
{
    Allocator a = tl_linear_allocator(4096);

    u8 *p = (u8*)ALLOC_A(a, 64, 1);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    void *sentinel = ALLOC(a, 16);
    ASSERT(sentinel != nullptr);

    u8 *r = (u8*)REALLOC(a, p, 64, 128);
    ASSERT(r != p);
    ASSERT(r != nullptr);
    for (i32 i = 0; i < 64; i++) ASSERT(r[i] == (u8)i);
}

TEST_PROC(memory__tl_linear__realloc_null_ptr_acts_as_alloc)
{
    Allocator a = tl_linear_allocator(4096);

    void *p = REALLOC(a, nullptr, 0, 64);
    ASSERT(p != nullptr);
}

TEST_PROC(memory__tl_linear__extend_zero_size_returns_nonnull)
{
    Allocator a = tl_linear_allocator(4096);
    void *p = ALLOC(a, 64);

    void *r = ALLOC_PROC(a, M_EXTEND, p, 64, 0, M_DEFAULT_ALIGN);
    ASSERT(r != nullptr);
}

TEST_PROC(memory__tl_linear__extend_last_alloc_is_inplace)
{
    Allocator a = tl_linear_allocator(4096);

    u8 *p = (u8*)ALLOC_A(a, 64, 1);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    u8 *r = (u8*)ALLOC_PROC(a, M_EXTEND, p, 64, 128, 1);
    ASSERT(r == p);
    for (i32 i = 0; i < 64; i++) ASSERT(r[i] == (u8)i);
}

TEST_PROC(memory__tl_linear__extend_non_last_alloc_does_not_copy_data)
{
    // Unlike realloc, extend of a non-last allocation allocates fresh memory
    // but does NOT copy. This is the documented distinction between M_REALLOC
    // and M_EXTEND: extend is for growing a buffer whose old contents are no
    // longer needed.
    Allocator a = tl_linear_allocator(4096);

    u8 *p = (u8*)ALLOC_A(a, 64, 1);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    void *sentinel = ALLOC(a, 16);
    ASSERT(sentinel != nullptr);

    u8 *r = (u8*)ALLOC_PROC(a, M_EXTEND, p, 64, 128, 1);
    ASSERT(r != nullptr);
    ASSERT(r != p);
    // Data is intentionally NOT checked -- extend semantics do not guarantee copy
}

TEST_PROC(memory__tl_linear__reset_to_start)
{
    Allocator a = tl_linear_allocator(4096);

    void *p0 = ALLOC(a, 64);
    ALLOC(a, 64);
    ALLOC(a, 64);

    RESET_ALLOC(a);

    void *p1 = ALLOC(a, 64);
    ASSERT(p1 == p0);
}

TEST_PROC(memory__tl_linear__reset_to_restore_point)
{
    Allocator a = tl_linear_allocator(4096);

    void *p0 = ALLOC(a, 64);
    void *restore_point = ALLOC(a, 1);

    void *p1 = ALLOC(a, 64);
    void *p2 = ALLOC(a, 64);

    RESTORE_ALLOC(a, restore_point);

    void *p3 = ALLOC(a, 64);
    ASSERT(p3 <= p1);
    (void)p0; (void)p2;
}

TEST_PROC(memory__tl_linear__alloc_beyond_capacity_panics)
{
    Allocator a = tl_linear_allocator(256);

    EXPECT_FAIL(ALLOC(a, 1024));
}

TEST_PROC(memory__tl_linear__get_allocator_info_reports_usage)
{
    SArena scratch = tl_scratch_arena();

    AllocatorInfo before = get_allocator_info(scratch);
    ASSERT(before.size > 0);

    void *p = ALLOC(scratch, 1024);
    ASSERT(p != nullptr);

    AllocatorInfo after = get_allocator_info(scratch);
    ASSERT(after.used >= before.used + 1024);
    ASSERT(after.size == before.size);
}

TEST_PROC(memory__linear__get_allocator_info_reports_usage)
{
    Allocator a = linear_allocator(256);

    AllocatorInfo before = get_allocator_info(a);
    ASSERT(before.size > 0);

    void *p = ALLOC(a, 64);
    ASSERT(p != nullptr);

    AllocatorInfo after = get_allocator_info(a);
    ASSERT(after.used >= before.used + 64);
    ASSERT(after.size == before.size);
}


TEST_PROC(memory__malloc__alloc_returns_nonnull)
{
    Allocator a = malloc_allocator();
    void *p = ALLOC(a, 64);
    ASSERT(p != nullptr);
    FREE(a, p);
}

TEST_PROC(memory__malloc__alloc_zero_size_returns_nonnull)
{
    Allocator a = malloc_allocator();
    void *p = ALLOC(a, 0);
    ASSERT(p != nullptr);
    FREE(a, p);
}

TEST_PROC(memory__malloc__alloc_respects_alignment)
{
    Allocator a = malloc_allocator();

    void *p4  = ALLOC_A(a, 32, 4);
    void *p8  = ALLOC_A(a, 32, 8);
    void *p16 = ALLOC_A(a, 32, 16);
    void *p32 = ALLOC_A(a, 32, 32);
    void *p64 = ALLOC_A(a, 32, 64);

    ASSERT(is_aligned(p4,  4));
    ASSERT(is_aligned(p8,  8));
    ASSERT(is_aligned(p16, 16));
    ASSERT(is_aligned(p32, 32));
    ASSERT(is_aligned(p64, 64));

    FREE(a, p4);
    FREE(a, p8);
    FREE(a, p16);
    FREE(a, p32);
    FREE(a, p64);
}

TEST_PROC(memory__malloc__free_null_is_safe)
{
    Allocator a = malloc_allocator();
    // Freeing nullptr must not crash (guarded by `if (old_ptr)` in malloc_alloc)
    void *ret = FREE(a, nullptr);
    ASSERT(ret == nullptr);
}

TEST_PROC(memory__malloc__realloc_preserves_data)
{
    Allocator a = malloc_allocator();

    u8 *p = (u8*)ALLOC(a, 64);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    u8 *r = (u8*)REALLOC(a, p, 64, 128);
    ASSERT(r != nullptr);
    for (i32 i = 0; i < 64; i++) ASSERT(r[i] == (u8)i);

    FREE(a, r);
}

TEST_PROC(memory__malloc__realloc_null_ptr_acts_as_alloc)
{
    Allocator a = malloc_allocator();
    void *p = REALLOC(a, nullptr, 0, 64);
    ASSERT(p != nullptr);
    FREE(a, p);
}

TEST_PROC(memory__malloc__extend_is_fresh_alloc_no_copy)
{
    Allocator a = malloc_allocator();

    u8 *p = (u8*)ALLOC(a, 64);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;

    u8 *r = (u8*)ALLOC_PROC(a, M_EXTEND, p, 64, 128, M_DEFAULT_ALIGN);
    ASSERT(r != nullptr);
    ASSERT(r != p);
    // Old pointer is NOT freed by extend -- caller is responsible.
    FREE(a, p);
    FREE(a, r);
}

TEST_PROC(memory__malloc__reset_logs_error_and_returns_null)
{
    Allocator a = malloc_allocator();
    EXPECT_FAIL(RESET_ALLOC(a));
}

TEST_PROC(memory__malloc__get_allocator_info_reports_zero_usage)
{
    Allocator a = malloc_allocator();

    AllocatorInfo info = get_allocator_info(a);
    ASSERT(info.size == 0);
    ASSERT(info.used == 0);
}

TEST_PROC(memory__malloc__get_allocator_info_stays_zero_after_allocations)
{
    Allocator a = malloc_allocator();

    void *p = ALLOC(a, 256);
    ASSERT(p != nullptr);

    AllocatorInfo info = get_allocator_info(a);
    ASSERT(info.size == 0);
    ASSERT(info.used == 0);

    FREE(a, p);
}

TEST_PROC(memory__vm_freelist__get_allocator_info_reports_capacity)
{
    Allocator a = vm_freelist_allocator(4 * MiB);

    AllocatorInfo info = get_allocator_info(a);
    ASSERT(info.size >= 4 * MiB);
    ASSERT(info.used == 0);
}

TEST_PROC(memory__vm_freelist__get_allocator_info_tracks_alloc_and_free)
{
    Allocator a = vm_freelist_allocator(4 * MiB);

    AllocatorInfo before = get_allocator_info(a);
    void *p = ALLOC(a, 4096);
    ASSERT(p != nullptr);

    AllocatorInfo after_alloc = get_allocator_info(a);
    ASSERT(after_alloc.used >= before.used + 4096);

    FREE(a, p);

    AllocatorInfo after_free = get_allocator_info(a);
    ASSERT(after_free.used == before.used);
    ASSERT(after_free.size == before.size);
}

TEST_PROC(memory__vm_freelist__near_exact_initial_alloc_succeeds)
{
    Allocator a = vm_freelist_allocator(4 * MiB);
    auto *state = (VMFreeListState*)a.state;

    void *p = ALLOC(a, vm_freelist_near_exact_page_request_size(state));
    ASSERT(p != nullptr);
    ASSERT(state->committed > 0);

    FREE(a, p);
}

TEST_PROC(memory__vm_freelist__free_then_exact_fit_reuses_committed_page)
{
    Allocator a = vm_freelist_allocator(4 * MiB);
    auto *state = (VMFreeListState*)a.state;

    i64 size = vm_freelist_exact_page_request_size(state);

    void *p0 = ALLOC(a, size);
    ASSERT(p0 != nullptr);
    i64 committed_after_first_alloc = state->committed;
    ASSERT(committed_after_first_alloc > 0);

    FREE(a, p0);
    ASSERT(state->committed == committed_after_first_alloc);

    void *p1 = ALLOC(a, size);
    ASSERT(p1 != nullptr);
    ASSERT(state->committed == committed_after_first_alloc);

    FREE(a, p1);
}

TEST_PROC(memory__vm_freelist__free_then_near_exact_fit_reuses_committed_page)
{
    Allocator a = vm_freelist_allocator(4 * MiB);
    auto *state = (VMFreeListState*)a.state;

    i64 exact_size = vm_freelist_exact_page_request_size(state);
    i64 near_size = vm_freelist_near_exact_page_request_size(state);

    void *p0 = ALLOC(a, exact_size);
    ASSERT(p0 != nullptr);

    FREE(a, p0);
    i64 committed_after_free = state->committed;
    ASSERT(committed_after_free > 0);

    void *p1 = ALLOC(a, near_size);
    ASSERT(p1 != nullptr);
    ASSERT(state->committed == committed_after_free);

    FREE(a, p1);
}

TEST_PROC(memory__vm_freelist__alternating_near_page_sizes_do_not_grow_committed)
{
    Allocator a = vm_freelist_allocator(4 * MiB);
    auto *state = (VMFreeListState*)a.state;

    i64 near_size = vm_freelist_near_exact_page_request_size(state);

    void *seed = ALLOC(a, vm_freelist_exact_page_request_size(state));
    ASSERT(seed != nullptr);
    FREE(a, seed);

    i64 committed_baseline = state->committed;
    ASSERT(committed_baseline > 0);

    for (i32 i = 0; i < 8; i++) {
        i64 size = (i & 1) ? near_size : vm_freelist_exact_page_request_size(state);

        void *p = ALLOC(a, size);
        ASSERT(p != nullptr);
        ASSERT(state->committed == committed_baseline);

        FREE(a, p);
        ASSERT(state->committed == committed_baseline);
    }
}


TEST_PROC(memory__scratch__alloc_returns_usable_memory)
{
    SArena scratch = tl_scratch_arena();

    u8 *p = (u8*)ALLOC(scratch, 64);
    ASSERT(p != nullptr);
    for (i32 i = 0; i < 64; i++) p[i] = (u8)i;
    for (i32 i = 0; i < 64; i++) ASSERT(p[i] == (u8)i);
}

TEST_PROC(memory__scratch__release_restores_to_restore_point)
{
    SArena outer = tl_scratch_arena();
    AllocatorInfo before = get_allocator_info(outer);

    {
        // Borrow the same arena through a second tl_scratch_arena call with no
        // conflict -- this should return the same underlying arena at a new
        // restore point.
        MArena inner = tl_scratch_arena(/*conflict=*/{});
        void *p = ALLOC(inner, 1024);
        ASSERT(p != nullptr);

        AllocatorInfo during = get_allocator_info(inner);
        ASSERT(during.used >= before.used + 1024);

        release_arena(&inner);
    }

    AllocatorInfo after = get_allocator_info(outer);
    ASSERT(after.used == before.used);
}

TEST_PROC(memory__scratch__no_conflict_reuses_same_underlying_arena)
{
    MArena a = tl_scratch_arena();
    release_arena(&a);

    MArena b = tl_scratch_arena();
    release_arena(&b);

    ASSERT(a.state == b.state);
}

TEST_PROC(memory__scratch__conflict_returns_different_arena)
{
    MArena a = tl_scratch_arena();
    MArena b = tl_scratch_arena(a);

    ASSERT(a.state != b.state);
    ASSERT(b.state != nullptr);

    release_arena(&b);
    release_arena(&a);
}

TEST_PROC(memory__scratch__non_scratch_conflict_still_returns_arena)
{
    Allocator heap = malloc_allocator();
    MArena a = tl_scratch_arena(heap);

    ASSERT(a.state != nullptr);
    ASSERT(a.proc != nullptr);

    void *p = ALLOC(a, 64);
    ASSERT(p != nullptr);

    release_arena(&a);
}


TEST_PROC(memory__scratch__nested_caller_callee_use_different_arenas)
{
    // Outer scope simulates the "caller": grabs arena 0.
    SArena caller_scratch = tl_scratch_arena();

    // Simulate a "callee" that receives caller_scratch as its output allocator
    // and needs its own temp arena.
    u8 *caller_alloc = (u8*)ALLOC(caller_scratch, 128);
    ASSERT(caller_alloc != nullptr);
    memset(caller_alloc, 0xCA, 128);

    {
        Allocator output_alloc = caller_scratch;  // the allocator the caller passed in
        SArena callee_scratch = tl_scratch_arena(output_alloc);  // must be arena 1

        ASSERT(callee_scratch.arena.state != caller_scratch.arena.state);

        // callee does temp work on its own scratch
        u8 *temp = (u8*)ALLOC(callee_scratch, 256);
        ASSERT(temp != nullptr);

        // callee writes result onto the output allocator (caller's arena)
        u8 *result = (u8*)ALLOC(output_alloc, 64);
        ASSERT(result != nullptr);

        // The inner (callee) temp allocation and the outer result must not overlap
        ASSERT(!is_overlapping(temp, 256, result, 64));
        // The inner temp and the pre-existing caller allocation must not overlap
        ASSERT(!is_overlapping(temp, 256, caller_alloc, 128));

        // callee_scratch releases here (RAII)
    }

    // Caller's pre-existing allocation must be intact after callee released its scratch
    for (i32 i = 0; i < 128; i++) ASSERT(caller_alloc[i] == 0xCA);

    AllocatorInfo info = get_allocator_info(caller_scratch);
    ASSERT(info.used >= 128 + 64);
}

TEST_PROC(memory__scratch__nested_inner_release_does_not_corrupt_outer)
{
    SArena outer = tl_scratch_arena();

    u8 *outer_data  = (u8*)ALLOC(outer, 64);
    u8 *outer_data2 = nullptr;
    ASSERT(outer_data != nullptr);
    memset(outer_data, 0xC0, 64);

    {
        SArena inner = tl_scratch_arena(outer);  // get the other arena
        ASSERT(inner.arena.state != outer.arena.state);

        // Inner allocates a large block -- must not overlap with outer_data
        u8 *inner_data = (u8*)ALLOC(inner, 128);
        ASSERT(inner_data != nullptr);
        ASSERT(!is_overlapping(inner_data, 128, outer_data, 64));

        // Fill with a canary to detect any cross-arena write corruption
        memset(inner_data, 0xCF, 128);

        // Allocate more from outer while inner is live -- still no overlap with inner
        outer_data2 = (u8*)ALLOC(outer, 64);
        ASSERT(outer_data2 != nullptr);
        ASSERT(!is_overlapping(inner_data, 128, outer_data2, 64));
        ASSERT(!is_overlapping(outer_data, 64, outer_data2, 64));
        memset(outer_data2, 0xC1, 64);

        // inner goes out of scope and releases here
    }

    // Outer data must be completely intact after inner was released
    for (i32 i = 0; i < 64; i++) ASSERT(outer_data[i]  == 0xC0);
    for (i32 i = 0; i < 64; i++) ASSERT(outer_data2[i] == 0xC1);
}

TEST_PROC(memory__scratch__nested_return_data_on_outer_survives_inner_release)
{
    // Full simulation of the canonical pattern:
    //
    //   outer_fn()
    //     scratch = tl_scratch_arena()          -- arena 0
    //     result = inner_fn(scratch)
    //     use(result)
    //
    //   inner_fn(Allocator mem)                 -- mem is arena 0
    //     scratch = tl_scratch_arena(mem)       -- arena 1
    //     temp = alloc(scratch, ...)            -- temp on arena 1
    //     result = alloc(mem, ...)              -- result on arena 0
    //     <scratch released>
    //     return result
    //
    SArena outer_scratch = tl_scratch_arena();
    Allocator output_mem = outer_scratch;

    u8 *pre_existing = (u8*)ALLOC(output_mem, 64);
    ASSERT(pre_existing != nullptr);
    memset(pre_existing, 0xCA, 64);

    u8 *returned_result = nullptr;

    {
        SArena inner_scratch = tl_scratch_arena(output_mem);
        ASSERT(inner_scratch.arena.state != outer_scratch.arena.state);

        u8 *temp_buf = (u8*)ALLOC(inner_scratch, 512);
        ASSERT(temp_buf != nullptr);
        memset(temp_buf, 0xCB, 512);

        returned_result = (u8*)ALLOC(output_mem, 32);
        ASSERT(returned_result != nullptr);
        memset(returned_result, 0xCC, 32);

        ASSERT(!is_overlapping(temp_buf, 512, returned_result, 32));
        ASSERT(!is_overlapping(temp_buf, 512, pre_existing, 64));
        ASSERT(!is_overlapping(returned_result, 32, pre_existing, 64));

        // inner_scratch releases here: arena 1 is rewound, temp_buf is gone
    }

    for (i32 i = 0; i < 32; i++) ASSERT(returned_result[i] == 0xCC);
    for (i32 i = 0; i < 64; i++) ASSERT(pre_existing[i]    == 0xCA);
}

TEST_PROC(memory__scratch__three_level_nesting_no_overlap)
{
    // L0 (this test body) uses heap as the permanent allocator and arena 0 as scratch.
    // L1 receives arena 0 as output, uses arena 1 as scratch.
    // L2 receives arena 1 as output, uses arena 0 as scratch.
    Allocator heap = malloc_allocator();

    SArena l0_scratch = tl_scratch_arena(heap);
    u8 *l0_temp = (u8*)ALLOC(l0_scratch, 128);
    ASSERT(l0_temp != nullptr);
    memset(l0_temp, 0xC0, 128);

    u8 *l0_result = (u8*)ALLOC(heap, 32);
    ASSERT(l0_result != nullptr);
    memset(l0_result, 0xC1, 32);

    {
        Allocator l1_output = l0_scratch;
        SArena l1_scratch = tl_scratch_arena(l1_output);
        ASSERT(l1_scratch.arena.state != l0_scratch.arena.state);

        u8 *l1_temp = (u8*)ALLOC(l1_scratch, 256);
        ASSERT(l1_temp != nullptr);
        memset(l1_temp, 0xC2, 256);

        u8 *l1_result = (u8*)ALLOC(l1_output, 48);
        ASSERT(l1_result != nullptr);
        memset(l1_result, 0xC3, 48);

        ASSERT(!is_overlapping(l0_temp, 128, l1_temp, 256));
        ASSERT(!is_overlapping(l0_temp, 128, l1_result, 48));
        ASSERT(!is_overlapping(l1_temp, 256, l1_result, 48));

        {
            Allocator l2_output = l1_scratch;
            SArena l2_scratch = tl_scratch_arena(l2_output);
            ASSERT(l2_scratch.arena.state != l1_scratch.arena.state);
            ASSERT(l2_scratch.arena.state == l0_scratch.arena.state);

            u8 *l2_temp = (u8*)ALLOC(l2_scratch, 64);
            ASSERT(l2_temp != nullptr);
            memset(l2_temp, 0xC4, 64);

            u8 *l2_result = (u8*)ALLOC(l2_output, 24);
            ASSERT(l2_result != nullptr);
            memset(l2_result, 0xC5, 24);

            ASSERT(!is_overlapping(l2_temp, 64, l1_temp, 256));
            ASSERT(!is_overlapping(l2_temp, 64, l2_result, 24));
            ASSERT(!is_overlapping(l1_temp, 256, l2_result, 24));
            ASSERT(!is_overlapping(l2_temp, 64, l0_temp, 128));
            ASSERT(!is_overlapping(l2_temp, 64, l1_result, 48));

            // l2_scratch releases -- arena 0 rewinds to l2's restore point
        }

        for (i32 i = 0; i < 128; i++) ASSERT(l0_temp[i] == 0xC0);
        for (i32 i = 0; i < 256; i++) ASSERT(l1_temp[i] == 0xC2);
        for (i32 i = 0; i < 48; i++) ASSERT(l1_result[i] == 0xC3);

        // l1_scratch releases -- arena 1 rewinds
    }

    for (i32 i = 0; i < 128; i++) ASSERT(l0_temp[i] == 0xC0);
    for (i32 i = 0; i < 32; i++) ASSERT(l0_result[i] == 0xC1);

    FREE(heap, l0_result);
}

TEST_PROC(memory__scratch__four_level_nesting_no_overlap)
{
    // L0: output = heap,    scratch = arena 0  (conflict = heap)
    // L1: output = arena 0, scratch = arena 1  (conflict = arena 0)
    // L2: output = arena 1, scratch = arena 0  (conflict = arena 1)
    // L3: output = arena 0, scratch = arena 1  (conflict = arena 0)
    //
    // Each level allocates a distinct-sized temp block so overlaps would be
    // detectable even if arena assignments were wrong.

    Allocator heap = malloc_allocator();

    SArena l0_scratch = tl_scratch_arena(heap);
    u8 *l0_temp   = (u8*)ALLOC(l0_scratch, 512);
    u8 *l0_result = (u8*)ALLOC(heap, 16);
    ASSERT(l0_temp   != nullptr);
    ASSERT(l0_result != nullptr);
    memset(l0_temp,   0xC0, 512);
    memset(l0_result, 0xC1, 16);

    {
        Allocator l1_output = l0_scratch;
        SArena l1_scratch = tl_scratch_arena(l1_output);
        ASSERT(l1_scratch.arena.state != l0_scratch.arena.state);

        u8 *l1_temp   = (u8*)ALLOC(l1_scratch, 384);
        u8 *l1_result = (u8*)ALLOC(l1_output, 32);
        ASSERT(l1_temp   != nullptr);
        ASSERT(l1_result != nullptr);
        memset(l1_temp,   0xC2, 384);
        memset(l1_result, 0xC3, 32);

        ASSERT(!is_overlapping(l0_temp, 512, l1_temp, 384));
        ASSERT(!is_overlapping(l1_temp, 384, l1_result, 32));
        ASSERT(!is_overlapping(l0_temp, 512, l1_result, 32));

        {
            Allocator l2_output = l1_scratch;
            SArena l2_scratch = tl_scratch_arena(l2_output);
            ASSERT(l2_scratch.arena.state == l0_scratch.arena.state);
            ASSERT(l2_scratch.arena.state != l1_scratch.arena.state);

            u8 *l2_temp   = (u8*)ALLOC(l2_scratch, 256);
            u8 *l2_result = (u8*)ALLOC(l2_output, 48);
            ASSERT(l2_temp   != nullptr);
            ASSERT(l2_result != nullptr);
            memset(l2_temp,   0xC4, 256);
            memset(l2_result, 0xC5, 48);

            ASSERT(!is_overlapping(l2_temp, 256, l1_temp, 384));
            ASSERT(!is_overlapping(l2_temp, 256, l2_result, 48));
            ASSERT(!is_overlapping(l1_temp, 384, l2_result, 48));
            ASSERT(!is_overlapping(l2_temp, 256, l0_temp,   512));
            ASSERT(!is_overlapping(l2_temp, 256, l1_result,  32));

            {
                Allocator l3_output = l2_scratch;
                SArena l3_scratch = tl_scratch_arena(l3_output);
                ASSERT(l3_scratch.arena.state == l1_scratch.arena.state);
                ASSERT(l3_scratch.arena.state != l0_scratch.arena.state);

                u8 *l3_temp   = (u8*)ALLOC(l3_scratch, 128);
                u8 *l3_result = (u8*)ALLOC(l3_output, 64);
                ASSERT(l3_temp   != nullptr);
                ASSERT(l3_result != nullptr);
                memset(l3_temp,   0xC6, 128);
                memset(l3_result, 0xC7, 64);

                u8 *all_ptrs[]  = { l0_temp, l1_result, l2_temp, l3_result, l1_temp, l2_result, l3_temp };
                i64 all_sizes[] = { 512,     32,        256,     64,        384,     48,         128    };
                i32 count = (i32)ARRAY_COUNT(all_ptrs);
                for (i32 x = 0; x < count; x++) {
                    for (i32 y = x+1; y < count; y++) {
                        ASSERT(!is_overlapping(all_ptrs[x], all_sizes[x], all_ptrs[y], all_sizes[y]));
                    }
                }

                // l3_scratch releases -- arena 1 rewinds to L3's restore point
            }

            for (i32 i = 0; i < 48; i++) ASSERT(l2_result[i] == 0xC5);

            // l2_scratch releases -- arena 0 rewinds to L2's restore point
        }

        for (i32 i = 0; i < 32; i++) ASSERT(l1_result[i] == 0xC3);

        // l1_scratch releases -- arena 1 rewinds
    }

    for (i32 i = 0; i < 512; i++) ASSERT(l0_temp[i] == 0xC0);
    for (i32 i = 0; i < 16; i++) ASSERT(l0_result[i] == 0xC1);

    FREE(heap, l0_result);
}

TEST_PROC(memory__scratch__sarena_raii_restores_on_scope_exit)
{
    SArena outer = tl_scratch_arena();
    AllocatorInfo before = get_allocator_info(outer);

    {
        SArena inner = tl_scratch_arena();
        ASSERT(inner.arena.state == outer.arena.state);

        void *p = ALLOC(inner, 4096);
        ASSERT(p != nullptr);

        AllocatorInfo mid = get_allocator_info(inner);
        ASSERT(mid.used >= before.used + 4096);
        // inner.~SArena() fires here, rewinding to inner's restore point
    }

    AllocatorInfo after = get_allocator_info(outer);
    ASSERT(after.used == before.used);
}

TEST_PROC(memory__scratch__alloc_before_and_after_inner_scope_both_survive)
{
    // Simulates:
    //   outer grabs arena 0, allocates temp
    //   inner grabs arena 0 (no conflict, new restore point), allocates and releases
    //   outer allocates temp2 -- must land after temp, not overwrite it
    //   both temp and temp2 must be intact
    SArena outer = tl_scratch_arena();

    u8 *temp = (u8*)ALLOC(outer, 64);
    ASSERT(temp != nullptr);
    memset(temp, 0xC0, 64);

    {
        SArena inner = tl_scratch_arena();
        ASSERT(inner.arena.state == outer.arena.state);

        u8 *inner_temp = (u8*)ALLOC(inner, 256);
        ASSERT(inner_temp != nullptr);
        ASSERT(inner_temp > temp);
        memset(inner_temp, 0xCF, 256);

        // inner.~SArena() -- rewinds to inner's restore point (just above temp)
    }

    u8 *temp2 = (u8*)ALLOC(outer, 64);
    ASSERT(temp2 != nullptr);
    ASSERT(!is_overlapping(temp, 64, temp2, 64));
    memset(temp2, 0xC1, 64);

    for (i32 i = 0; i < 64; i++) ASSERT(temp[i]  == 0xC0);
    for (i32 i = 0; i < 64; i++) ASSERT(temp2[i] == 0xC1);
}

TEST_PROC(memory__scratch__sarena_nested_scopes_lifo_ordering)
{
    SArena s0 = tl_scratch_arena();
    AllocatorInfo info0 = get_allocator_info(s0);

    void *p0 = ALLOC(s0, 64);
    ASSERT(p0 != nullptr);

    {
        SArena s1 = tl_scratch_arena();
        ASSERT(s1.arena.state == s0.arena.state);

        void *p1 = ALLOC(s1, 64);
        ASSERT(p1 != nullptr);
        ASSERT(p1 > p0);

        {
            SArena s2 = tl_scratch_arena();
            ASSERT(s2.arena.state == s0.arena.state);

            void *p2 = ALLOC(s2, 64);
            ASSERT(p2 != nullptr);
            ASSERT(p2 > p1);

            AllocatorInfo info2 = get_allocator_info(s2);
            ASSERT(info2.used >= info0.used + 64 + 64 + 64);
            // s2.~SArena() -- rewinds to s2's restore point (above p1)
        }

        AllocatorInfo after2 = get_allocator_info(s1);
        ASSERT(after2.used == get_allocator_info(s1).used);

        // s1.~SArena() -- rewinds to s1's restore point (above p0)
    }

    AllocatorInfo after1 = get_allocator_info(s0);
    ASSERT(after1.used >= info0.used + 64);
    ASSERT(after1.used < info0.used + 64 + 64);
}

TEST_PROC(memory__scratch__multiple_allocs_accumulate_before_release)
{
    SArena scratch = tl_scratch_arena();

    AllocatorInfo start = get_allocator_info(scratch);

    void *a = ALLOC(scratch, 100);
    void *b = ALLOC(scratch, 200);
    void *c = ALLOC(scratch, 300);
    ASSERT(a != nullptr && b != nullptr && c != nullptr);

    AllocatorInfo mid = get_allocator_info(scratch);
    ASSERT(mid.used >= start.used + 600);

    ASSERT(a != b && b != c && a != c);
}


TEST_PROC(memory__arena__restore_resets_to_creation_point)
{
    SArena scratch = tl_scratch_arena();
    AllocatorInfo before = get_allocator_info(scratch);

    // Grab another handle to the same arena at the current position.
    MArena snap = tl_scratch_arena();
    ASSERT(snap.state == scratch.arena.state);

    void *p = ALLOC(snap, 1024);
    ASSERT(p != nullptr);

    AllocatorInfo after_alloc = get_allocator_info(snap);
    ASSERT(after_alloc.used >= before.used + 1024);

    restore_arena(&snap);

    AllocatorInfo after_restore = get_allocator_info(snap);
    ASSERT(after_restore.used == before.used);
}

TEST_PROC(memory__arena__get_info_size_is_constant)
{
    SArena scratch = tl_scratch_arena();

    AllocatorInfo i0 = get_allocator_info(scratch);
    ALLOC(scratch, 1024);
    AllocatorInfo i1 = get_allocator_info(scratch);
    ALLOC(scratch, 1024);
    AllocatorInfo i2 = get_allocator_info(scratch);

    // Total capacity must never change
    ASSERT(i0.size == i1.size);
    ASSERT(i1.size == i2.size);
}

TEST_PROC(memory__arena__get_info_used_tracks_allocations)
{
    SArena scratch = tl_scratch_arena();

    AllocatorInfo before = get_allocator_info(scratch);

    ALLOC(scratch, 256);
    AllocatorInfo after1 = get_allocator_info(scratch);
    ASSERT(after1.used >= before.used + 256);

    ALLOC(scratch, 512);
    AllocatorInfo after2 = get_allocator_info(scratch);
    ASSERT(after2.used >= after1.used + 512);
}


TEST_PROC(memory__buffer__write_and_read_back_typed)
{
    u8 storage[256];
    MemoryBuffer buf{ storage, (i32)sizeof storage, 0 };

    write_memory(&buf, (i32)42);
    write_memory(&buf, (f32)3.14f);
    write_memory(&buf, (u64)0xDEADBEEFCAFEBABEULL);

    buf.offset = 0;  // rewind

    i32 iv = read_memory<i32>(&buf);
    f32 fv = read_memory<f32>(&buf);
    u64 uv = read_memory<u64>(&buf);

    ASSERT(iv == 42);
    ASSERT(fv == 3.14f);
    ASSERT(uv == 0xDEADBEEFCAFEBABEULL);
}

TEST_PROC(memory__buffer__offset_advances_correctly)
{
    u8 storage[64];
    MemoryBuffer buf{ storage, (i32)sizeof storage, 0 };

    ASSERT(buf.offset == 0);
    write_memory(&buf, (i32)1);
    ASSERT(buf.offset == (i32)sizeof(i32));
    write_memory(&buf, (i32)2);
    ASSERT(buf.offset == (i32)(2 * sizeof(i32)));

    buf.offset = 0;
    read_memory<i32>(&buf);
    ASSERT(buf.offset == (i32)sizeof(i32));
}

TEST_PROC(memory__buffer__read_raw_bytes_returns_correct_pointer)
{
    u8 storage[64];
    for (i32 i = 0; i < 64; i++) storage[i] = (u8)i;

    MemoryBuffer buf{ storage, 64, 0 };

    void *ptr = read_memory(&buf, 8);
    ASSERT(ptr == storage);       // should point to beginning
    ASSERT(buf.offset == 8);      // offset advanced by 8

    ptr = read_memory(&buf, 8);
    ASSERT(ptr == storage + 8);
    ASSERT(buf.offset == 16);
}

TEST_PROC(memory__buffer__read_beyond_end_panics)
{
    u8 storage[16];
    MemoryBuffer buf{ storage, 16, 0 };

    buf.offset = 12;

    EXPECT_FAIL(read_memory(&buf, 8));
}

TEST_PROC(memory__buffer__write_beyond_end_panics)
{
    u8 storage[16];
    MemoryBuffer buf{ storage, 16, 0 };

    buf.offset = 12;

    EXPECT_FAIL(write_memory(&buf, storage, 8));
}

TEST_PROC(memory__buffer__typed_read_beyond_end_panics)
{
    u8 storage[4];
    MemoryBuffer buf{ storage, 4, 0 };

    buf.offset = 2;

    EXPECT_FAIL(read_memory<i32>(&buf));
}

TEST_PROC(memory__buffer__typed_write_beyond_end_panics)
{
    u8 storage[4];
    MemoryBuffer buf{ storage, 4, 0 };

    buf.offset = 2;

    EXPECT_FAIL(write_memory<i32>(&buf, 42));
}


TEST_PROC(memory__macros__alloc_t_constructs_typed_object)
{
    Allocator a = malloc_allocator();

    struct Point { i32 x, y; };
    Point *p = ALLOC_T(a, Point){ .x = 10, .y = 20 };

    ASSERT(p != nullptr);
    ASSERT(p->x == 10);
    ASSERT(p->y == 20);
    ASSERT(is_aligned(p, alignof(Point)));

    FREE(a, p);
}

TEST_PROC(memory__macros__alloc_arr_allocates_typed_array)
{
    Allocator a = malloc_allocator();

    i32 *arr = ALLOC_ARR(a, i32, 8);
    ASSERT(arr != nullptr);
    ASSERT(is_aligned(arr, alignof(i32)));

    for (i32 i = 0; i < 8; i++) arr[i] = i * i;
    for (i32 i = 0; i < 8; i++) ASSERT(arr[i] == i * i);

    FREE(a, arr);
}

TEST_PROC(memory__macros__realloc_arr_preserves_existing_data)
{
    Allocator a = malloc_allocator();

    i32 *arr = ALLOC_ARR(a, i32, 4);
    ASSERT(arr != nullptr);
    for (i32 i = 0; i < 4; i++) arr[i] = i + 1;

    // Grow: existing 4 elements must survive
    arr = REALLOC_ARR(a, i32, arr, 4, 8);
    ASSERT(arr != nullptr);
    for (i32 i = 0; i < 4; i++) ASSERT(arr[i] == i + 1);

    FREE(a, arr);
}

TEST_PROC(memory__macros__alloc_t_on_arena_constructs_object)
{
    SArena scratch = tl_scratch_arena();

    struct Vec3 { f32 x, y, z; };
    Vec3 *v = ALLOC_T(scratch, Vec3){ .x = 1.0f, .y = 2.0f, .z = 3.0f };

    ASSERT(v != nullptr);
    ASSERT(v->x == 1.0f);
    ASSERT(v->y == 2.0f);
    ASSERT(v->z == 3.0f);
    ASSERT(is_aligned(v, alignof(Vec3)));
}

TEST_PROC(memory__macros__alloc_t_runs_default_initialisers)
{
    Allocator a = malloc_allocator();

    struct Point { f32 v = 10.0f; };
    Point *p = ALLOC_T(a, Point);

    ASSERT(p != nullptr);
    ASSERT(p->v == 10.0f);

    FREE(a, p);
}

TEST_PROC(memory__macros__alloc_t_default_init_can_be_overridden)
{
    Allocator a = malloc_allocator();

    struct Point { f32 v = 10.0f; };
    Point *p = ALLOC_T(a, Point){ .v = 99.0f };

    ASSERT(p != nullptr);
    ASSERT(p->v == 99.0f);

    FREE(a, p);
}

TEST_PROC(memory__macros__alloc_t_on_arena_runs_default_initialisers)
{
    SArena scratch = tl_scratch_arena();

    struct Counts { i32 hits = 0; i32 misses = 0; };
    Counts *c = ALLOC_T(scratch, Counts);

    ASSERT(c != nullptr);
    ASSERT(c->hits   == 0);
    ASSERT(c->misses == 0);
}

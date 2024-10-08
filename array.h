#ifndef ARRAY_H
#define ARRAY_H

#include "core.h"
#include "memory.h"

#include <type_traits>
#include <initializer_list>

// In honour of Dirk
#define ANON_ARRAY(fields) struct anon_##__LINE__ { fields; }; Array<anon_##__LINE__>

// -------------------------------------------------------------------------------
template<typename T>
struct Array {
    T *data = nullptr;
    i32 count = 0;

    constexpr T& operator[](i32 i)
    {
        ASSERT(i < count && i >= 0);
        return data[i];
    }

    T& at(i32 i)
    {
        ASSERT(i < count && i >= 0);
        return data[i];
    }

    T* begin() { return &data[0]; }
    T* end() { return &data[count]; }

    bool operator==(const Array<T> &other) const
    {
        if (this->count != other.count) return false;
        if (this->data == other.data) return true;
        for (i32 i = 0; i < this->count; i++) if (this->data[i] != other.data[i]) return false;
        return true;
    }
};

template<typename T>
struct ArrayIterator {
    struct iter {
        T *ptr;
        i32 index;

        T& elem() { return ptr[index]; }

        operator T*() { return &ptr[index]; }
        operator T&() { return ptr[index]; }
        T* operator->() { return &ptr[index]; }
        iter operator*() { return *this; }
        bool operator!=(iter other) { return index != other.index; }

        iter operator++() { index++; return *this; }
        iter operator--() { index--; return *this; }
    };

    Array<T> arr;

    iter begin() { return { .ptr = arr.begin(), .index = 0 }; }
    iter end() { return { .ptr = arr.begin(), .index = arr.count }; }
};

template<typename T>
struct ReverseIterator {
    Array<T> arr;

    struct iter {
        T *ptr;
        i32 index;

        T& elem() { return ptr[index]; }

        operator T*() { return &ptr[index]; }
        operator T&() { return ptr[index]; }
        T* operator->() { return &ptr[index]; }
        iter operator*() { return *this; }
        bool operator!=(iter other) { return index != other.index; }

        iter operator++() { index--; return *this; }
        iter operator--() { index++; return *this; }

    };

    iter begin() { return { .ptr = arr.begin(), .index = arr.count-1 }; }
    iter end() { return { .ptr = arr.begin(), .index = -1 }; }
};

template<typename T>
ArrayIterator<T> iterator(Array<T> arr) { return { .arr = arr }; }

template<typename T>
ReverseIterator<T> reverse(Array<T> arr) { return { .arr = arr }; }

template<typename T>
T array_pop(Array<T> *arr)
{
    ASSERT(arr->count > 0);
    return arr->data[--arr->count];
}

template<typename T>
T* array_tail(Array<T> &arr)
{
    if (arr.count == 0) return nullptr;
    return &arr.data[arr.count-1];
}

template<typename T>
void array_create(Array<T> *arr, i32 count, Allocator mem)
{
    T *nptr = ALLOC_ARR(mem, T, count);
    if (arr->count > 0) {
        for (i32 i = 0; i < arr->count; i++) nptr[i] = arr->data[i];
    }

    arr->data = nptr;
    arr->count = count;
}

template<typename T>
Array<T> array_create(i32 count, Allocator mem)
{
    return { .data = ALLOC_ARR(mem, T, count), .count = count };
}

template<typename T>
void array_remove_unsorted(Array<T> *arr, i32 index)
{
    ASSERT(index >= 0);
    ASSERT(index < arr->count);

    arr->data[index] = arr->data[arr->count-1];
    arr->count--;
}

template<typename T>
void array_remove(Array<T> *arr, i32 index)
{
    ASSERT(index >= 0);
    ASSERT(index < arr->count);

    memmove(&arr->data[index], &arr->data[index+1], (arr->count-index-1)*sizeof(T));
    arr->count--;
}

template<typename T>
Array<T> slice(Array<T> arr, i32 start, i32 end)
{
    ASSERT(start <= end);
    ASSERT(start <= arr.count);
    ASSERT(end <= arr.count);
    return { .data = &arr.data[start], .count = end-start };
}

template<typename T>
Array<T> slice(Array<T> arr, i32 start)
{
    ASSERT(start <= arr.count);
    return { .data = &arr.data[start], .count = arr.count-start };
}

template<typename T>
i32 array_find_index(Array<T> arr, T value)
{
    for (i32 i = 0; i < arr.count; i++) if (arr[i] == value) return i;
    return -1;
}

template<typename T, typename E>
i32 array_find_index(Array<T> arr, E value)
{
    for (i32 i = 0; i < arr.count; i++) if (arr[i] == value) return i;
    return -1;
}

template<typename T, typename E>
T* array_find(Array<T> arr, E value)
{
    i32 i = array_find_index(arr, value);
    if (i >= 0) return &arr[i];
    return nullptr;
}

template<typename T, typename E>
bool array_contains(Array<T> arr, E value)
{
    return array_find_index(arr, value) >= 0;
}


template<typename T>
void array_swap(Array<T> arr, i32 a, i32 b)
{
    SWAP(arr[a], arr[b]);
}

template<typename T, typename K>
void array_swap(Array<T> arr, i32 a, i32 b, Array<K> arr1)
{
    SWAP(arr[a], arr[b]);
    SWAP(arr1[a], arr1[b]);
}

template<typename T, typename K, typename... Tail>
void array_swap(Array<T> arr, i32 a, i32 b, Array<K> arr1, Array<Tail>... tail)
{
    SWAP(arr[a], arr[b]);
    array_swap(arr1, a, b, tail...);
}


template<typename T, typename... Tail>
void exchange_sort(Array<T> arr, Array<Tail>... tail)
{
    for (int i = 0; i < arr.count; i++) {
        for (int j = i+1; j < arr.count; j++) {
            if (arr[i] > arr[j]) array_swap(arr, i, j, tail...);
        }
    }

}

template<typename T, typename... Tail>
void quick_sort_desc(Array<T> arr, i32 l, i32 r, Array<Tail>... tail)
{
    if (l < 0 || r < 0 || l >= r) return;

    T pivot = arr[(r+l)/2];

    i32 i = l-1;
    i32 j = r+1;

    i32 pi = -1;
    while (true) {
        do i += 1; while(arr[i] > pivot);
        do j -= 1; while(arr[j] < pivot);

        if (i >= j) {
            pi = j;
            break;
        }

        array_swap(arr, j, i, tail...);
    }

    ASSERT(pi >= 0);

    quick_sort_desc(arr, l, pi, tail...);
    quick_sort_desc(arr, pi+1, r, tail...);
}

template<typename T, typename... Tail>
void quick_sort_asc(Array<T> arr, i32 l, i32 r, Array<Tail>... tail)
{
    if (l < 0 || r < 0 || l >= r) return;

    T pivot = arr[(r+l)/2];

    i32 i = l-1;
    i32 j = r+1;

    i32 pi = -1;
    while (true) {
        do i += 1; while(arr[i] < pivot);
        do j -= 1; while(arr[j] > pivot);

        if (i >= j) {
            pi = j;
            break;
        }

        array_swap(arr, i, j, tail...);
    }

    ASSERT(pi >= 0);

    quick_sort_asc(arr, l, pi, tail...);
    quick_sort_asc(arr, pi+1, r, tail...);
}

template<typename T, typename... Tail>
void quick_sort_desc(Array<T> arr, Array<Tail>... tail)
{
    quick_sort_desc(arr, 0, arr.count-1, tail...);
}

template<typename T, typename... Tail>
void quick_sort_asc(Array<T> arr, Array<Tail>... tail)
{
    quick_sort_asc(arr, 0, arr.count-1, tail...);
}

// -------------------------------------------------------------------------------
template<typename T>
struct DynamicArray : Array<T> {
    i32 capacity = 0;
    Allocator alloc = {};
};

template<typename T>
void array_grow(DynamicArray<T> *arr, i32 additional_elements)
{
    if (!arr->alloc.proc) arr->alloc = mem_dynamic;
    if (arr->capacity >= arr->count+additional_elements) return;

    i32 old_capacity = arr->capacity;
    arr->capacity = MAX(arr->count+additional_elements, arr->capacity*2);
    arr->data = REALLOC_ARR(arr->alloc, T, arr->data, old_capacity, arr->capacity);
}

template<typename T>
void array_reserve(DynamicArray<T> *arr, i32 capacity)
{
    if (!arr->alloc.proc) arr->alloc = mem_dynamic;
    if (arr->capacity < capacity) array_grow(arr, capacity-arr->capacity);
}

template<typename T>
void array_resize(DynamicArray<T> *arr, i32 count)
{
    if (!arr->alloc.proc) arr->alloc = mem_dynamic;
    arr->count = count;
    array_reserve(arr, count);
}

template<typename T>
void array_set_grow(DynamicArray<T> *arr, i32 index, T element)
{
    if (arr->capacity <= index+1) {
        array_grow(arr, index-arr->capacity+1);

        for (i32 i = arr->count; i < index; i++) {
            arr->data[i] = {};
        }
    }

    arr->count = MAX(arr->count, index+1);
    arr->at(index) = element;
}

template<typename T>
DynamicArray<T> array_duplicate(Array<T> src, Allocator mem)
{
    DynamicArray<T> dst{ .alloc = mem };
    array_copy(&dst, src);
    return src;
}

template<typename T>
void array_reset(DynamicArray<T> *arr, i32 reserve_capacity = 0)
{
    arr->count = arr->capacity = 0;
    arr->data = nullptr;

    if (reserve_capacity > 0) array_reserve(arr, reserve_capacity);
}

template<typename T>
void array_clear(DynamicArray<T> *arr)
{
    arr->count = 0;
}

template<typename T>
void array_destroy(DynamicArray<T> *arr)
{
    ASSERT(arr->alloc.proc);
    if (arr->capacity > 0) FREE(arr->alloc, arr->data);
    arr->data = nullptr;
    arr->count = arr->capacity = 0;
}

template<typename T>
void array_destroy_reset(DynamicArray<T> *arr, Allocator mem, i32 reserve_capacity = 0)
{
    ASSERT(arr->alloc.proc);
    if (arr->capacity > 0) FREE(arr->alloc, arr->data);
    arr->data = nullptr;
    arr->count = arr->capacity = 0;

    arr->alloc = mem;
    if (reserve_capacity > 0) array_reserve(arr, reserve_capacity);
}

template<typename T>
i32 array_add(DynamicArray<T> *arr, const T& e)
{
    array_grow(arr, 1);
    arr->data[arr->count] = e;
    return arr->count++;
}

inline i32 array_add(DynamicArray<const char*> *arr, const char *e)
{
    array_grow(arr, 1);
    arr->data[arr->count] = e;
    return arr->count++;
}

template<typename T>
i32 array_add(DynamicArray<T> *arr, const T *es, i32 count)
{
    array_grow(arr, count);
    memcpy(&arr->data[arr->count], es, count*sizeof *es);
    i32 start_index = arr->count;
    arr->count += count;
    return start_index;
}

template<typename T>
i32 array_add(DynamicArray<T> *arr, const Array<T> es)
{
    array_grow(arr, es.count);
    memcpy(&arr->data[arr->count], es.data, es.count*sizeof(T));
    i32 start_index = arr->count;
    arr->count += es.count;
    return start_index;
}

template<typename T>
i32 array_append(DynamicArray<T> *arr, std::initializer_list<T> list)
{
    Array<T> l{ .data = (T*)list.begin(), .count = (i32)list.size() };
    return array_add(arr, l);
}

template<typename T>
void array_copy(DynamicArray<T> *dst, const Array<T> src)
{
    array_resize(dst, src.count);
    memcpy(dst->data, src.data, src.count*sizeof(T));
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const T& e)
{
    ASSERT(insert_at <= arr->count);
    array_grow(arr, 1);

    for (i32 i = arr->count; i > insert_at; i--) arr->data[i] = arr->data[i-1];
    arr->data[insert_at] = e;
    arr->count++;
    return insert_at;
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const T *es, i32 count)
{
    ASSERT(insert_at <= arr->count);
    array_grow(arr, count);

    for (i32 i = arr->count+count-1, j = insert_at;
         j < arr->count;
         j++,i--)
    {
        arr->data[i] = arr->data[j];
    }

    for (i32 i = 0; i < count; i++)
        arr->data[insert_at+i] = es[i];

    arr->count += count;
    return insert_at;
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const Array<T> es)
{
    ASSERT(insert_at >= 0);
    ASSERT(insert_at <= arr->count);
    array_grow(arr, es.count);

    for (i32 i = arr->count+es.count-1; i > insert_at; i--) arr->data[i] = arr->data[i-1];
    for (i32 i = 0; i < es.count; i++) arr->data[insert_at+i] = es[i];
    arr->count += es.count;
    return insert_at;
}


template<typename T>
i32 array_replace(DynamicArray<T> *arr, i32 start, i32 end, const Array<T> values)
{
    ASSERT(start >= 0);
    ASSERT(end >= 0);

    i32 remove_count = end-start;

    i32 old_count = arr->count;
    i32 new_count = arr->count - remove_count + values.count;
    array_reserve(arr, new_count);

    if (remove_count > 0 && values.count > remove_count) {
        i32 move_count = new_count-end-1;
        for (i32 i = 0; i < move_count; i++) arr->data[new_count-i-1] = arr->data[old_count-i-1];
    }

    for (i32 i = 0; i < values.count; i++) arr->data[start+i] = values.data[i];

    if (remove_count > values.count) {
        for (i32 i = start+values.count; i < new_count; i++) arr->data[i] = arr->data[i+1];
    }

    arr->count = new_count;
    return new_count;
}

template<typename T>
i32 array_replace(DynamicArray<T> *arr, i32 start, i32 end, std::initializer_list<T> values)
{
    return array_replace(arr, start, end, { .data = (T*)values.begin(), .count = (i32)values.size() });
}


template<typename T, typename E>
T* array_find_emplace(DynamicArray<T> *arr, const E& value)
{
    i32 i = array_find_index(*arr, value);
    if (i >= 0) return &arr->at(i);

    i = array_add(arr, T{});
    return &arr->at(i);
}

// -------------------------------------------------------------------------------
template<typename T, i32 N>
struct FixedArray {
    using capacity = std::integral_constant<i32, N>;

private:
    alignas(alignof(T)) u8 storage[sizeof(T)*N];

public:
    i32 count;

    constexpr T& operator[](i32 i)
    {
        ASSERT(i < count && i >= 0);
        return *(T*)&storage[i*sizeof(T)];
    }

    constexpr T* data(i32 i = 0) { return &((T*)storage)[i]; }

    T* begin() { return (T*)&storage[0]; }
    T* end()   { return (T*)&storage[count*sizeof(T)]; }

    const T* begin() const { return (const T*)&storage[0]; }
    const T* end()   const { return (const T*)&storage[count*sizeof(T)]; }

    bool operator==(const FixedArray<T, N> &other) const
    {
        if (this->count != other.count) return false;
        if (this->storage == other.storage) return true;

        auto *lhs = begin();
        auto *rhs = other.begin();
        for (i32 i = 0; i < this->count; i++) {
            if (lhs[i] != rhs[i]) return false;
        }
        return true;
    }

    FixedArray() = default;
    FixedArray(const FixedArray<T, N> &other) : count(other.count)
    {
        auto *dst = begin();
        auto *src = other.begin();
        for (i32 i = 0; i < other.count; i++) dst[i] = src[i];
    }

    FixedArray<T, N>& operator=(const FixedArray<T, N> &other)
    {
        this->count = other.count;
        auto *dst = begin();
        auto *src = other.begin();
        for (i32 i = 0; i < count; i++) dst[i] = src[i];
        return *this;
    }

    FixedArray(Array<T> arr) : count(MIN(N, arr.count))
    {
        auto *dst = begin();
        for (i32 i = 0; i < count; i++) dst[i] = arr.data[i];
    }

    FixedArray(std::initializer_list<T> list) : count(MIN(N, list.size()))
    {
        auto *dst = begin();
        auto *src = list.begin();
        for (i32 i = 0; i < count; i++) dst[i] = src[i];
    }

    FixedArray<T, N>& operator=(const Array<T> &arr)
    {
        this->count = MIN(N, arr.count);
        auto *dst = begin();
        for (i32 i = 0; i < arr.count; i++) dst[i] = arr.data[i];
        return *this;
    }
};

template<typename T, i32 N>
ArrayIterator<T> iterator(FixedArray<T, N> &arr) { return { { .data = arr.data(), .count = arr.count } }; }

template<typename T, i32 N>
ReverseIterator<T> reverse(FixedArray<T, N> &arr) { return { { .data = arr.data(), .count = arr.count } }; }

template<typename T, i32 N>
i32 array_add(FixedArray<T, N> *arr, T e)
{
    PANIC_IF(arr->count >= N, "FixedArray overflow");
    *arr->data(arr->count) = e;
    return arr->count++;
}

template<typename T, i32 N>
i32 array_add(FixedArray<T, N> *arr, T *es, i32 count)
{
    PANIC_IF(arr->count+count > N, "FixedArray overflow");
    memcpy(arr->data(arr->count), es, count*sizeof *es);
    arr->count += count;
    return arr->count;
}

template<typename T, i32 N>
T array_pop(FixedArray<T, N> *arr)
{
    ASSERT(arr->count > 0);
    return *arr->data(--arr->count);
}

template<typename T, i32 N>
T* array_tail(FixedArray<T, N> &arr)
{
    if (arr.count == 0) return nullptr;
    return arr.data(arr.count-1);
}

template<typename T, i32 N>
Array<T> slice(FixedArray<T, N> &arr, i32 start, i32 end)
{
    ASSERT(start <= end);
    ASSERT(start <= arr.count);
    ASSERT(end <= arr.count);
    return { .data = arr.data(start), .count = end-start };
}

template<typename T, i32 N>
Array<T> slice(FixedArray<T, N> &arr, i32 start)
{
    ASSERT(start <= arr.count);
    return { .data = arr.data(start), .count = arr.count-start };
}


// -------------------------------------------------------------------------------
template<typename T, i32 N>
struct StaticArray : Array<T> {
    T storage[N];

    StaticArray() : Array<T>{ .data = storage, .count = N } {}

    StaticArray(StaticArray<T, N> &other) :
        Array<T>{ .data = storage, .count = N }
    {
        memcpy(storage, other.data, N * sizeof storage[0]);
    }

    StaticArray(StaticArray<T, N> &&other) :
        Array<T>{ .data = storage, .count = N }
    {
        memcpy(storage, other.data, N * sizeof storage[0]);
    }

    StaticArray(Array<T> arr) :
        Array<T>{ .data = storage, .count = N }
    {
        memcpy(storage, arr.data, MIN(N, arr.count) * sizeof storage[0]);
    }

    StaticArray(std::initializer_list<T> list) :
        Array<T>{ .data = storage, .count = N }
    {
        memcpy(storage, list.begin(), MIN(N, list.size()) * sizeof storage[0]);
    }


    StaticArray<T, N>& operator=(StaticArray<T, N> &other)
    {
        this->data = this->storage;
        this->count = N;
        memcpy(storage, other.data,  N * sizeof storage[0]);
        return *this;
    }

    StaticArray<T, N>& operator=(StaticArray<T, N> &&other)
    {
        this->data = this->storage;
        this->count = N;
        memcpy(storage, other, N * sizeof storage[0]);
        return *this;
    }

    StaticArray<T, N>& operator=(Array<T> arr)
    {
        this->data = this->storage;
        this->count = N;
        memcpy(storage, arr.data, MIN(N, arr.count) * sizeof storage[0]);
        return *this;
    }
};

/// test suite
#include "test.h"
static TEST_PROC(indexing, CATEGORY(array))
{
    i32 data[5] = { 1, 2, 3, 4, 5 };
    Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data)-1 };

    ASSERT(arr.count == 4);
    ASSERT(arr[0] == 1);
    EXPECT_FAIL(arr[4] = 6);

    ASSERT(array_tail(arr) == &arr[3]);
    ASSERT(array_tail(arr) == &data[3]);
}

static TEST_PROC(find, CATEGORY(array))
{
    i32 data[5] = { 1, 2, 3, 4, 5 };
    Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data) };

    ASSERT(*array_find(arr, 1) == 1);
    ASSERT(*array_find(arr, 5) == 5);
    ASSERT(array_find(arr, 6) == nullptr);

    ASSERT(array_find(arr, 1) == &data[0]);
    ASSERT(array_find(arr, 1) == &arr[0]);
    ASSERT(array_find(arr, 5) == &data[4]);
    ASSERT(array_find(arr, 5) == &arr[4]);

    ASSERT(array_find_index(arr, 1) == 0);
    ASSERT(array_find_index(arr, 5) == 4);
    ASSERT(array_find_index(arr, 6) == -1);
}

static TEST_PROC(append, CATEGORY(dynamic_array))
{
    DynamicArray<i32> arr{};
    ASSERT(arr.alloc.proc == nullptr);

    array_add(&arr, 1);
    ASSERT(arr.alloc == mem_dynamic);
    ASSERT(arr.count == 1);
    ASSERT(arr[0] == 1);

    array_append(&arr, { 2, 3 });
    ASSERT(arr.count == 3);
    ASSERT(arr[1] == 2);
    ASSERT(arr[2] == 3);

    i32 ints[2] = { 4, 5 };
    array_add(&arr, ints, ARRAY_COUNT(ints));
    ASSERT(arr.count == 5);
    ASSERT(arr[3] == 4);
    ASSERT(arr[4] == 5);
}

static TEST_PROC(insert, CATEGORY(dynamic_array))
{
    DynamicArray<i32> arr{};
    ASSERT(arr.alloc.proc == nullptr);

    EXPECT_FAIL(array_insert(&arr, 1, 1));
    ASSERT(arr.count == 0);

    array_insert(&arr, 0, 1);
    ASSERT(arr.count == 1);
    ASSERT(arr[0] == 1);

    array_insert(&arr, 0, 2);
    ASSERT(arr.count == 2);
    ASSERT(arr[0] == 2);
    ASSERT(arr[1] == 1);

    i32 ints[2] = { 3, 4 };
    array_insert(&arr, 1, ints, ARRAY_COUNT(ints));
    ASSERT(arr.count == 4);
    ASSERT(arr[0] == 2);
    ASSERT(arr[1] == 3);
    ASSERT(arr[2] == 4);
    ASSERT(arr[3] == 1);
}

static TEST_PROC(set, CATEGORY(dynamic_array))
{
    {
        DynamicArray<i32> arr{};
        array_set_grow(&arr, 0, 1);
        ASSERT(arr.count == 1);
        ASSERT(arr.capacity >= 1);
        ASSERT(arr[0] == 1);

        array_set_grow(&arr, 3, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr.capacity >= 4);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 0);
        ASSERT(arr[2] == 0);
        ASSERT(arr[3] == 2);
    }
}


#endif // ARRAY_H

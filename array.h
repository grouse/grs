#ifndef ARRAY_H
#define ARRAY_H

#include "core.h"
#include "memory.h"

#include <type_traits>
#include <initializer_list>

// In honour of Dirk
#define ANON_ARRAY(fields) struct anon_##__LINE__ { fields; }; Array<anon_##__LINE__>

// -- structures
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
struct DynamicArray : Array<T> {
    i32 capacity = 0;
    Allocator alloc = {};
};

template<typename T, i32 N>
struct FixedArray : Array<T> {
    using capacity = std::integral_constant<i32, N>;
    alignas(T) u8 storage[sizeof(T)*N];

    FixedArray() { this->data = (T*)storage; }

    FixedArray(const FixedArray<T, N> &other)
    {
        this->data  = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
    }

    template<i32 M>
    FixedArray(const FixedArray<T, M> &other)
    {
        this->data  = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
    }

    FixedArray(const Array<T> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
    }

    FixedArray(std::initializer_list<T> list)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, list.size());
        auto *src = list.begin();
        for (i32 i = 0; i < this->count; i++) this->data[i] = src[i];
    }

    FixedArray<T, N>& operator=(const FixedArray<T, N> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
        return *this;
    }

    template<i32 M>
    FixedArray<T, N>& operator=(const FixedArray<T, M> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
        return *this;
    }

    FixedArray<T, N>& operator=(const Array<T> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        for (i32 i = 0; i < this->count; i++) this->data[i] = other.data[i];
        return *this;
    }

    FixedArray<T, N>& operator=(std::initializer_list<T> list)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, list.size());

        auto *src = list.begin();
        for (i32 i = 0; i < this->count; i++) this->data[i] = src[i];
        return *this;
    }
};

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



// -- iterators
template<typename T>
struct ArrayIterator {
    Array<T> arr;

    struct Proxy {
        T *ptr;
        i32 index;

        T& elem() { return ptr[index]; }

        operator T*()     { return &ptr[index]; }
        operator T&()     { return ptr[index]; }
        T* operator->()   { return &ptr[index]; }
        Proxy operator*() { return *this; }

        bool operator==(Proxy other) { return ptr == other.ptr && index == other.index; }
        bool operator!=(Proxy other) { return ptr != other.ptr || index != other.index; }

        Proxy& operator++() { index++; return *this; }
        Proxy& operator--() { index--; return *this; }
    };

    Proxy begin() { return { arr.begin(), 0         }; }
    Proxy end()   { return { arr.begin(), arr.count }; }
};

template<typename T>
struct ArrayIterator<T*> {
    Array<T*> arr;

    struct Proxy {
        T **ptr;
        i32 index;

        T*& elem()        { return ptr[index]; }
        operator T*()     { return ptr[index]; }
        operator T&()     { return *ptr[index]; }
        T* operator->()   { return ptr[index]; }
        Proxy operator*() { return *this; }

        bool operator==(Proxy other) { return ptr == other.ptr && index == other.index; }
        bool operator!=(Proxy other) { return ptr != other.ptr || index != other.index; }

        Proxy& operator++() { index++; return *this; }
        Proxy& operator--() { index--; return *this; }
    };

    Proxy begin() { return { arr.begin(), 0         }; }
    Proxy end()   { return { arr.begin(), arr.count }; }
};

template<typename T>
struct ReverseIterator {
    Array<T> arr;

    struct Proxy {
        T *ptr;
        i32 index;

        T& elem()         { return ptr[index]; }
        operator T*()     { return &ptr[index]; }
        operator T&()     { return ptr[index]; }
        T* operator->()   { return &ptr[index]; }
        Proxy operator*() { return *this; }

        bool operator==(Proxy other) { return ptr == other.ptr && index == other.index; }
        bool operator!=(Proxy other) { return index != other.index; }

        Proxy& operator++() { index--; return *this; }
        Proxy& operator--() { index++; return *this; }
    };

    Proxy begin() { return { arr.begin(), arr.count-1 }; }
    Proxy end()   { return { arr.begin(), -1          }; }
};

template<typename T>
struct ReverseIterator<T*> {
    Array<T*> arr;

    struct Proxy {
        T **ptr;
        i32 index;

        T*& elem()        { return ptr[index]; }
        operator T*()     { return ptr[index]; }
        operator T&()     { return *ptr[index]; }
        T* operator->()   { return ptr[index]; }
        Proxy operator*() { return *this; }

        bool operator==(Proxy other) { return ptr == other.ptr && index == other.index; }
        bool operator!=(Proxy other) { return index != other.index; }

        Proxy& operator++() { index--; return *this; }
        Proxy& operator--() { index++; return *this; }
    };

    Proxy begin() { return { arr.begin(), arr.count-1 }; }
    Proxy end()   { return { arr.begin(), -1          }; }
};

template<typename T>
ArrayIterator<T> iterator(Array<T> arr) { return { .arr = arr }; }

template<typename T>
ReverseIterator<T> reverse(Array<T> arr) { return { .arr = arr }; }

// -- array procedures
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
    ASSERT(start >= 0);
    return { .data = &arr.data[start], .count = end-start };
}

template<typename T>
Array<T> slice(Array<T> arr, i32 start)
{
    ASSERT(start <= arr.count);
    ASSERT(start >= 0);
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

// -- dynamic array procedures
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

// -- fixed array procedures
template<typename T, i32 N>
void array_reset(FixedArray<T, N> *arr)
{
    arr->count = 0;
}

template<typename T, i32 N>
i32 array_add(FixedArray<T, N> *arr, T e)
{
    PANIC_IF(arr->count >= N, "FixedArray overflow");
    arr->data[arr->count] = e;
    return arr->count++;
}

template<typename T, i32 N>
i32 array_add(FixedArray<T, N> *arr, T *es, i32 count)
{
    PANIC_IF(arr->count+count > N, "FixedArray overflow");
    memcpy(&arr->data[arr->count], es, count*sizeof *es);
    arr->count += count;
    return arr->count;
}

// -- test suite
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

static TEST_PROC(iterator, CATEGORY(array))
{
    i32 data[5] = { 1, 2, 3, 4, 5 };
    Array<i32> arr{ .data = data, .count = ARRAY_COUNT(data) };

    {
        i32 index = 0;
        for (auto it : arr) {
            ASSERT(it == arr[index]);
            index++;
        }
    }

    {
        i32 index = 0;
        for (auto it : iterator(arr)) {
            ASSERT(it == arr[index]);
            ASSERT(it.index == index);
            index++;
        }
    }

    {
        i32 index = 0;
        for (auto it : reverse(arr)) {
            ASSERT(it == arr[arr.count-index-1]);
            ASSERT(it.index == arr.count-index-1);
            index++;
        }
    }

    {
        i32 *ptrs[5] = { &data[0], &data[1], &data[2], &data[3], &data[4] };
        Array<i32*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : ptrs_arr) {
            ASSERT(it == ptrs[index]);
            ASSERT(it == &data[index]);
            index++;
        }

    }

    {
        struct D { i32 x; };
        D sdata[5] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
        D *ptrs[5] = { &sdata[0], &sdata[1], &sdata[2], &sdata[3], &sdata[4] };
        Array<D*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : ptrs_arr) {
            ASSERT(it == ptrs[index]);
            ASSERT(it == &sdata[index]);
            ASSERT(it->x == sdata[index].x);
            index++;
        }
    }

    {
        struct D { i32 x; };
        D sdata[5] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
        D *ptrs[5] = { &sdata[0], &sdata[1], &sdata[2], &sdata[3], &sdata[4] };
        Array<D*> ptrs_arr{ .data = ptrs, .count = ARRAY_COUNT(ptrs) };

        i32 index = 0;
        for (auto it : iterator(ptrs_arr)) {
            ASSERT(it.index == index);
            ASSERT(it == ptrs[index]);
            ASSERT(it == &sdata[index]);
            ASSERT(it->x == sdata[index].x);
            index++;
        }

    }
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

static TEST_PROC(basic_construction, CATEGORY(fixed_array))
{
    // Test default construction
    FixedArray<int, 5> arr1;
    ASSERT(arr1.count == 0);
    ASSERT(arr1.data == (int*)arr1.storage);

    // Test initialization list construction
    FixedArray<int, 5> arr2 = {1, 2, 3};
    ASSERT(arr2.count == 3);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr2[0] == 1);
    ASSERT(arr2[1] == 2);
    ASSERT(arr2[2] == 3);

    // Test construction from Array
    int raw_data[] = {4, 5, 6, 7};
    Array<int> source{raw_data, 4};
    FixedArray<int, 5> arr3(source);
    ASSERT(arr3.count == 4);
    ASSERT(arr3.data == (int*)arr3.storage);
    for (int i = 0; i < 4; i++) {
        ASSERT(arr3[i] == raw_data[i]);
    }
}

static TEST_PROC(copy_operations, CATEGORY(fixed_array))
{
    {
        FixedArray<int, 5> original = {1, 2, 3};
        ASSERT(original.data == (int*)original.storage);

        FixedArray<int, 5> copied(original);
        ASSERT(copied.data == (int*)copied.storage);
        ASSERT(copied.count == 3);

        for (int i = 0; i < 3; i++) {
            ASSERT(copied[i] == original[i]);
        }

        copied[0] = 10;
        ASSERT(original[0] == 1);
        ASSERT(copied[0] == 10);
    }

    {
        FixedArray<int, 5> original = {1, 2, 3};
        ASSERT(original.data == (int*)original.storage);

        FixedArray<int, 5> assigned;
        assigned = original;
        ASSERT(assigned.data == (int*)assigned.storage);
        ASSERT(assigned.count == 3);

        for (int i = 0; i < 3; i++) {
            ASSERT(assigned[i] == original[i]);
        }

        assigned[0] = 10;
        ASSERT(original[0] == 1);
        ASSERT(assigned[0] == 10);
    }
}

static TEST_PROC(size_limits, CATEGORY(fixed_array))
{
    // Test truncation when source is larger than capacity
    int raw_data[] = {1, 2, 3, 4, 5, 6};
    Array<int> large_source{raw_data, 6};
    FixedArray<int, 4> truncated(large_source);
    ASSERT(truncated.count == 4);
    for (int i = 0; i < 4; i++) {
        ASSERT(truncated[i] == raw_data[i]);
    }

    // Test initialization list truncation
    FixedArray<int, 3> truncated_init = {1, 2, 3, 4, 5};
    ASSERT(truncated_init.count == 3);
    for (int i = 0; i < 3; i++) {
        ASSERT(truncated_init[i] == i + 1);
    }
}

static TEST_PROC(data_pointer_integrity, CATEGORY(fixed_array))
{
    FixedArray<int, 5> arr1 = {1, 2, 3};
    const int* original_data = arr1.data;

    FixedArray<int, 5> arr2(arr1);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr1.data == original_data);

    FixedArray<int, 5> arr3;
    arr3 = arr1;
    ASSERT(arr3.data == (int*)arr3.storage);
    ASSERT(arr1.data == original_data);

    FixedArray<int, 5> arr4;
    arr4 = arr3 = arr2 = arr1;
    ASSERT(arr4.data == (int*)arr4.storage);
    ASSERT(arr3.data == (int*)arr3.storage);
    ASSERT(arr2.data == (int*)arr2.storage);
    ASSERT(arr1.data == original_data);
}

static TEST_PROC(pop, CATEGORY(array))
{
    {
        int data[] = {1, 2, 3, 4};
        Array<int> arr{data, 4};

        ASSERT(array_pop(&arr) == 4);
        ASSERT(arr.count == 3);
        ASSERT(array_pop(&arr) == 3);
        ASSERT(arr.count == 2);
    }
}

static TEST_PROC(tail, CATEGORY(array))
{
    // Test basic tail access
    {
        int data[] = {1, 2, 3};
        Array<int> arr{data, 3};

        ASSERT(*array_tail(arr) == 3);
        arr.count--;
        ASSERT(*array_tail(arr) == 2);
    }

    // Test empty array
    {
        Array<int> empty{};
        ASSERT(array_tail(empty) == nullptr);
    }
}

static TEST_PROC(create, CATEGORY(array))
{
    // Test creation with default allocator
    {
        Array<int> arr = array_create<int>(5, mem_dynamic);
        ASSERT(arr.count == 5);
        ASSERT(arr.data != nullptr);

        // Test data can be written to
        for (int i = 0; i < 5; i++) {
            arr.data[i] = i;
            ASSERT(arr[i] == i);
        }
    }
}

static TEST_PROC(remove, CATEGORY(array))
{
    // Test remove (ordered)
    {
        int data[] = {1, 2, 3, 4, 5};
        Array<int> arr{data, 5};

        array_remove(&arr, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr[0] == 1);
        ASSERT(arr[1] == 2);
        ASSERT(arr[2] == 4);
        ASSERT(arr[3] == 5);

        array_remove(&arr, 0);
        ASSERT(arr.count == 3);
        ASSERT(arr[0] == 2);
        ASSERT(arr[1] == 4);
        ASSERT(arr[2] == 5);

        EXPECT_FAIL(array_remove(&arr, 3)); // Out of bounds
    }

    // Test remove_unsorted (unordered)
    {
        int data[] = {1, 2, 3, 4, 5};
        Array<int> arr{data, 5};

        array_remove_unsorted(&arr, 2);
        ASSERT(arr.count == 4);
        ASSERT(arr[2] == 5);

        array_remove_unsorted(&arr, 0);
        ASSERT(arr.count == 3);

        EXPECT_FAIL(array_remove_unsorted(&arr, 3)); // Out of bounds
    }
}

static TEST_PROC(slice, CATEGORY(array))
{
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr{data, 5};

    // Test full slice
    {
        Array<int> sliced = slice(arr, 0, 5);
        ASSERT(sliced.count == 5);
        ASSERT(sliced.data == arr.data);
    }

    // Test partial slice
    {
        Array<int> sliced = slice(arr, 1, 4);
        ASSERT(sliced.count == 3);
        ASSERT(sliced.data == &arr.data[1]);
        ASSERT(sliced[0] == 2);
        ASSERT(sliced[1] == 3);
        ASSERT(sliced[2] == 4);
    }

    // Test single-argument slice
    {
        Array<int> sliced = slice(arr, 2);
        ASSERT(sliced.count == 3);
        ASSERT(sliced.data == &arr.data[2]);
    }

    // Test edge cases
    {
        Array<int> empty = slice(arr, 5, 5);
        ASSERT(empty.count == 0);

        EXPECT_FAIL(slice(arr, 2, 1)); // Invalid range
        EXPECT_FAIL(slice(arr, -1));   // Invalid start
        EXPECT_FAIL(slice(arr, 0, 6)); // Invalid end
    }
}

static TEST_PROC(find, CATEGORY(array))
{
    int data[] = {1, 2, 3, 2, 1};
    Array<int> arr{data, 5};

    // Test array_find_index
    {
        ASSERT(array_find_index(arr, 1) == 0); // First occurrence
        ASSERT(array_find_index(arr, 2) == 1);
        ASSERT(array_find_index(arr, 4) == -1); // Not found
    }

    // Test array_find
    {
        ASSERT(*array_find(arr, 1) == 1);
        ASSERT(array_find(arr, 1) == &arr[0]);
        ASSERT(array_find(arr, 4) == nullptr);
    }

    // Test array_contains
    {
        ASSERT(array_contains(arr, 1));
        ASSERT(array_contains(arr, 3));
        ASSERT(!array_contains(arr, 4));
    }
}

static TEST_PROC(swap, CATEGORY(array))
{
    // Test single array swap
    {
        int data[] = {1, 2, 3};
        Array<int> arr{data, 3};

        array_swap(arr, 0, 2);
        ASSERT(arr[0] == 3);
        ASSERT(arr[2] == 1);
    }

    // Test multi-array swap
    {
        int data1[] = {1, 2, 3};
        char data2[] = {'a', 'b', 'c'};
        Array<int> arr1{data1, 3};
        Array<char> arr2{data2, 3};

        array_swap(arr1, 0, 2, arr2);
        ASSERT(arr1[0] == 3 && arr1[2] == 1);
        ASSERT(arr2[0] == 'c' && arr2[2] == 'a');
    }
}

static TEST_PROC(sort, CATEGORY(array))
{
    // Test exchange sort
    {
        int data[] = {5, 3, 1, 4, 2};
        Array<int> arr{data, 5};

        exchange_sort(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] <= arr[i+1]);
        }
    }

    // Test quick sort ascending
    {
        int data[] = {5, 3, 1, 4, 2};
        Array<int> arr{data, 5};

        quick_sort_asc(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] <= arr[i+1]);
        }
    }

    // Test quick sort descending
    {
        int data[] = {1, 3, 5, 4, 2};
        Array<int> arr{data, 5};

        quick_sort_desc(arr);
        for (int i = 0; i < arr.count-1; i++) {
            ASSERT(arr[i] >= arr[i+1]);
        }
    }

    // Test sorting with parallel arrays
    {
        int keys[] = {5, 3, 1, 4, 2};
        char values[] = {'a', 'b', 'c', 'd', 'e'};
        Array<int> key_arr{keys, 5};
        Array<char> value_arr{values, 5};

        quick_sort_asc(key_arr, value_arr);
        ASSERT(key_arr[0] == 1 && value_arr[0] == 'c');
        ASSERT(key_arr[4] == 5 && value_arr[4] == 'a');
    }
}

#endif // ARRAY_H

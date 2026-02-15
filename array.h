#ifndef ARRAY_H
#define ARRAY_H

#include "core.h"
#include "memory.h"

#include <initializer_list>

#ifndef ASSERT_BOUNDS
#define ASSERT_BOUNDS(i, min, max) do { ASSERT(i <= max); ASSERT(i >= min); } while(0)
#endif

// In honour of Dirk
#define ANON_ARRAY(fields) struct anon_##__LINE__ { fields; }; Array<anon_##__LINE__>
#define ARRAY(arr) (Array{ &arr[0], ARRAY_COUNT(arr) })

// -- structures
template<typename T>
struct Array {
    T *data = nullptr;
    i32 count = 0;

    constexpr T& operator[](i32 i)
    {
        ASSERT_BOUNDS(i, 0, count-1);
        return data[i];
    }

    T& at(i32 i)
    {
        ASSERT_BOUNDS(i, 0, count-1);
        return data[i];
    }

    T* begin() { return &data[0]; }
    T* end() { return &data[count]; }

    const T* begin() const { return &data[0]; }
    const T* end() const { return &data[count]; }

    bool operator==(const Array<T> &other) const
    {
        if (this->count != other.count) return false;
        if (this->data == other.data) return true;
        for (i32 i = 0; i < this->count; i++) if (this->data[i] != other.data[i]) return false;
        return true;
    }

    explicit operator bool() const { return count; }
};

template<typename T>
struct DynamicArray : Array<T> {
    i32 capacity = 0;
    Allocator alloc = {};
};

template<typename T, i32 N>
struct FixedArray : Array<T> {
    alignas(T) u8 storage[sizeof(T)*N];

    constexpr i32 capacity() { return N; }

    FixedArray()
    {
        this->data = (T*)storage;
        this->count = 0;
    }

    FixedArray(const FixedArray<T, N> &other)
    {
        this->data  = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
    }

    template<i32 M>
    FixedArray(const FixedArray<T, M> &other)
    {
        this->data  = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
    }

    FixedArray(const Array<T> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
    }

    FixedArray(std::initializer_list<T> list)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, list.size());
        auto *src = list.begin();
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(src[i]);
    }

    FixedArray<T, N>& operator=(const FixedArray<T, N> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return *this;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
        return *this;
    }

    template<i32 M>
    FixedArray<T, N>& operator=(const FixedArray<T, M> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return *this;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
        return *this;
    }

    FixedArray<T, N>& operator=(const Array<T> &other)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, other.count);
        if (this->data == other.data) return this;
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(other.data[i]);
        return *this;
    }

    FixedArray<T, N>& operator=(std::initializer_list<T> list)
    {
        this->data = (T*)this->storage;
        this->count = MIN(N, list.size());

        auto *src = list.begin();
        for (i32 i = 0; i < this->count; i++) new (&this->data[i]) T(src[i]);
        return *this;
    }
};


// -- iterators
template<typename T>
struct ArrayIterator {
    T* arr;
    i32 count;

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

    Proxy begin() { return { arr, 0 }; }
    Proxy end()   { return { arr, count }; }
};

template<typename T>
struct ArrayIterator<T*> {
    T** arr;
    i32 count;

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

    Proxy begin() { return { arr, 0 }; }
    Proxy end()   { return { arr, count }; }
};

template<typename T>
struct ReverseIterator {
    T *arr;
    i32 count;

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

    Proxy begin() { return { arr, count-1 }; }
    Proxy end()   { return { arr, -1 }; }
};

template<typename T>
struct ReverseIterator<T*> {
    T** arr;
    i32 count;

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

    Proxy begin() { return { arr, count-1 }; }
    Proxy end()   { return { arr, -1 }; }
};

template<typename T>
ArrayIterator<T> iterator(Array<T> arr) { return { arr.data, arr.count }; }

template<typename T>
ReverseIterator<T> reverse(Array<T> arr) { return { arr.data, arr.count }; }

template<typename T, i32 N>
ArrayIterator<T> iterator(T (&arr)[N]) { return { arr, N }; }

template<typename T, i32 N>
ReverseIterator<T> reverse(T (&arr)[N]) { return { arr, N }; }

// -- array procedures
template<typename T>
Array<T> array(T *data, i32 count) { return Array<T>{ data, count }; }

template<typename T>
T array_pop(Array<T> *arr)
{
    ASSERT_BOUNDS(arr->count, 1, arr->count);
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
        for (i32 i = 0; i < arr->count; i++) new (&nptr[i]) T(arr->data[i]);
    }
    for (i32 i = arr->count; i < count; i++) new (&nptr[i]) T{};

    arr->data = nptr;
    arr->count = count;
}

template<typename T>
Array<T> array_create(i32 count, Allocator mem)
{
    Array<T> arr = { .data = ALLOC_ARR(mem, T, count), .count = count };
    for (auto &it : arr) new (&it) T{};
    return arr;
}

template<typename T>
void array_remove_unsorted(Array<T> *arr, i32 index)
{
    ASSERT_BOUNDS(index, 0, arr->count-1);
    arr->data[index] = arr->data[arr->count-1];
    arr->count--;
}

template<typename T>
void array_remove(Array<T> *arr, i32 index)
{
    ASSERT_BOUNDS(index, 0, arr->count-1);
    memmove(&arr->data[index], &arr->data[index+1], (arr->count-index-1)*sizeof(T));
    arr->count--;
}

template<typename T>
Array<T> slice(Array<T> arr, i32 start, i32 end)
{
    ASSERT_BOUNDS(start, 0, end);
    ASSERT_BOUNDS(end, 0, arr.count);
    return { .data = &arr.data[start], .count = end-start };
}

template<typename T>
Array<T> slice(Array<T> arr, i32 start)
{
    ASSERT_BOUNDS(start, 0, arr.count);
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
    arr->capacity = MAX(arr->count+additional_elements, old_capacity*2);
    T *nptr = EXTEND_ARR(arr->alloc, T, arr->data, old_capacity, arr->capacity);

    if (nptr != arr->data) {
        for (i32 i = 0; i < arr->count; i++) new (&nptr[i]) T(arr->data[i]);
        FREE(arr->alloc, arr->data);
    }

    arr->data = nptr;
}

template<typename T>
void array_reserve(DynamicArray<T> *arr, i32 capacity)
{
    if (!arr->alloc.proc) arr->alloc = mem_dynamic;
    if (arr->capacity < capacity) array_grow(arr, capacity-arr->count);
}

template<typename T>
void array_resize(DynamicArray<T> *arr, i32 count)
{
    if (!arr->alloc.proc) arr->alloc = mem_dynamic;
    array_reserve(arr, count);
    for (i32 i = arr->count; i < count; i++) new (&arr->data[i]) T();
    arr->count = count;
}

template<typename T>
void array_set_grow(DynamicArray<T> *arr, i32 index, T element)
{
    if (arr->capacity <= index+1) {
        array_grow(arr, index-arr->capacity+1);

        for (i32 i = arr->count; i < index; i++) {
            new (&arr->data[i]) T{};
        }
    }

    arr->count = MAX(arr->count, index+1);
    new (&arr->data[index]) T(element);
}

template<typename T>
DynamicArray<T> array_duplicate(Array<T> src, Allocator mem)
{
    DynamicArray<T> dst{ .alloc = mem };
    array_copy(&dst, src);
    return dst;
}

template<typename T>
void array_reset(DynamicArray<T> *arr, i32 reserve_capacity = 0)
{
    if (arr->data) {
        FREE(arr->alloc, arr->data);
        arr->data = nullptr;
    }

    arr->count = arr->capacity = 0;

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
    new (&arr->data[arr->count]) T(e);
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
    for (i32 i = 0; i < count; i++) new (&arr->data[arr->count+i]) T(es[i]);
    i32 start_index = arr->count;
    arr->count += count;
    return start_index;
}

template<typename T>
i32 array_add(DynamicArray<T> *arr, const Array<T> es)
{
    array_grow(arr, es.count);
    for (i32 i = 0; i < es.count; i++) new (&arr->data[arr->count+i]) T(es.data[i]);
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
    for (i32 i = 0; i < src.count; i++) new (&dst->data[i]) T(src.data[i]);
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const T& e)
{
    ASSERT_BOUNDS(insert_at, 0, arr->count);
    array_grow(arr, 1);

    for (i32 i = arr->count; i > insert_at; i--) arr->data[i] = arr->data[i-1];
    new (&arr->data[insert_at]) T(e);
    arr->count++;
    return insert_at;
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const T *es, i32 count)
{
    ASSERT_BOUNDS(insert_at, 0, arr->count);
    array_grow(arr, count);

    for (i32 i = arr->count+count-1, j = insert_at;
         j < arr->count;
         j++,i--)
    {
        arr->data[i] = arr->data[j];
    }

    for (i32 i = 0; i < count; i++)
        new (&arr->data[insert_at+i]) T(es[i]);

    arr->count += count;
    return insert_at;
}

template<typename T>
i32 array_insert(DynamicArray<T> *arr, i32 insert_at, const Array<T> es)
{
    ASSERT_BOUNDS(insert_at, 0, arr->count);
    array_grow(arr, es.count);

    for (i32 i = arr->count+es.count-1; i > insert_at; i--) arr->data[i] = arr->data[i-1];
    for (i32 i = 0; i < es.count; i++) new (&arr->data[insert_at+i]) T(es[i]);
    arr->count += es.count;
    return insert_at;
}


template<typename T>
i32 array_replace(DynamicArray<T> *arr, i32 start, i32 end, const Array<T> values)
{
    ASSERT_BOUNDS(start, 0, arr->count);
    ASSERT_BOUNDS(end, 0, arr->count);

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
    new (&arr->data[arr->count]) T(e);
    return arr->count++;
}

template<typename T, i32 N>
i32 array_add(FixedArray<T, N> *arr, T *es, i32 count)
{
    PANIC_IF(arr->count+count > N, "FixedArray overflow");
    for (i32 i = 0; i < count; i++) new (&arr->data[arr->count+i]) T(es[i]);
    arr->count += count;
    return arr->count;
}

#endif // ARRAY_H

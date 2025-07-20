#ifndef MAP_H
#define MAP_H

#include "core.h"
#include "memory.h"
#include "hash.h"

#define DYNAMIC_MAP_INNITIAL_CAPACITY 16
#define DYNAMIC_MAP_LOAD_FACTOR 0.5

template<typename K, typename V>
struct DynamicMap {
    struct Pair {
        K key;
        V value;
        bool occupied;

        V* operator->() { return &value; }

        operator V&() { return value; }
        operator V*() { return &value; }
    };

    struct Iterator {
        DynamicMap *table;
        i32 slot;

        Iterator operator++()
        {
            slot++;
            while (slot < table->capacity && !table->slots[slot].occupied) slot++;
            return *this;
        }

        bool operator!=(const Iterator &other) { return slot != other.slot; }

        Pair& operator*() { return table->slots[slot]; }
        Pair* operator->() { return &table->slots[slot]; }
    };

    Pair *slots = nullptr;
    i32 count = 0;
    i32 capacity = 0;
    Allocator alloc = {};

    Iterator begin()
    {
        Iterator it = { this, 0 };
        while (it.slot < capacity && !slots[it.slot].occupied) it.slot++;
        return it;
    }

    Iterator end()
    {
        Iterator it = { this, capacity };
        return it;
    }
};

template<typename K, typename V>
void map_reset(DynamicMap<K, V> *map)
{
    map->slots = nullptr;
    map->count = 0;
    map->capacity = 0;
}

template<typename V>
void map_reset(DynamicMap<String, V> *map, Allocator mem)
{
    if (!map->alloc) map->alloc = mem;
    PANIC_IF(map->alloc != mem, "mismatching allocators");

    for (i32 i = 0; i  < map->capacity; i++) {
        if (!map->slots[i].occupied) continue;
        FREE(map->alloc, map->slots[i].key.data);
    }

    map->slots = nullptr;
    map->count = 0;
    map->capacity = 0;
}

template<typename K, typename V>
void map_set(DynamicMap<K, V> *map, const K &key, const V &value);

template<typename K, typename V>
i32 map_find_slot(DynamicMap<K, V> *map, const K &key)
{
    // TODO(jesper): round robin to reduce the upper bound of the probing
    if (map->capacity == 0) return -1;
    i32 i = hash32(key) % map->capacity;
    i32 end_probe = i;
    while (map->slots[i].occupied && map->slots[i].key != key) {
        i = (i+1) % map->capacity;
        if (i == end_probe) return -1;
    }

    return i;
}

template<typename K, typename V>
void map_grow(DynamicMap<K, V> *map, i32 new_capacity)
{
    //LOG_INFO("[map][%p] grow: %d -> %d", map, map->capacity, new_capacity);
    ASSERT(new_capacity > map->capacity);
    if (map->alloc.proc == nullptr) map->alloc = mem_dynamic;

    using Pair = typename DynamicMap<K,V>::Pair;
    DynamicMap<K, V> old_table = *map;

    map->slots = ALLOC_ARR(map->alloc, Pair, new_capacity);
    for (i32 i = 0; i < new_capacity; i++) map->slots[i].occupied = false;
    map->capacity = new_capacity;
    map->count = 0;

    for (i32 i = 0; i < old_table.capacity; i++) {
        if (!old_table.slots[i].occupied) continue;
        map_set(map, old_table.slots[i].key, old_table.slots[i].value);
    }

    if (old_table.slots) FREE(map->alloc, old_table.slots);
}

template<typename K, typename V>
i32 map_set_slot(DynamicMap<K, V> *map, i32 slot, const K &key, const V &value)
{
    if (map->count >= map->capacity*DYNAMIC_MAP_LOAD_FACTOR) {
        i32 new_capacity = map->capacity == 0 ? DYNAMIC_MAP_INNITIAL_CAPACITY : map->capacity*2;
        map_grow(map, new_capacity);

        slot = map_find_slot(map, key);
        ASSERT(slot >= 0);
    }

    //LOG_INFO("[map][%p] set slot[%d]", map, slot);
    map->slots[slot].occupied = true;
    new (&map->slots[slot].value) V(value);
    new (&map->slots[slot].key) K(key);
    map->count++;

    return slot;
}

template<typename K, typename V, i32 n>
i32 map_set_slot(DynamicMap<K, V[n]> *map, i32 slot, const K &key, const V (&value)[n])
{
    if (map->count >= map->capacity*DYNAMIC_MAP_LOAD_FACTOR) {
        i32 new_capacity = map->capacity == 0 ? DYNAMIC_MAP_INNITIAL_CAPACITY : map->capacity*2;
        map_grow(map, new_capacity);

        slot = map_find_slot(map, key);
        ASSERT(slot >= 0);
    }

    //LOG_INFO("[map][%p] set slot[%d]", map, slot);
    map->slots[slot].occupied = true;
    new (&map->slots[slot].key) K(key);
    for (i32 i = 0; i < n; i++) new (&map->slots[slot].value[i]) V(value[i]);
    map->count++;

    return slot;
}

template<typename K, typename V>
void map_set(DynamicMap<K, V> *map, const K &key, const V &value)
{
    i32 slot = map_find_slot(map, key);
    if (slot >= 0 && map->slots[slot].occupied) {
        //LOG_INFO("[map] set slot[%d]", slot);
        map->slots[slot].value = value;
        return;
    }

    map_set_slot(map, slot, key, value);
}

template<typename K, typename V, i32 n>
void map_set(DynamicMap<K, V[n]> *map, const K &key, const V (&value)[n])
{
    i32 slot = map_find_slot(map, key);
    if (slot >= 0 && map->slots[slot].occupied) {
        for (i32 i = 0; i < n; i++) map->slots[slot].value[i] = value[i];
        return;
    }

    map_set_slot(map, slot, key, value);
}

template<typename K, typename V>
void map_remove(DynamicMap<K, V> *map, const K &key)
{
    i32 slot = map_find_slot(map, key);
    if (slot == -1 || !map->slots[slot].occupied) return;
    map->slots[slot].occupied = false;
    map->count--;
}

template<typename K, typename V>
V* map_find(DynamicMap<K, V> *map, const K &key)
{
    i32 i = map_find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value;
}

template<typename K, typename V, i32 n>
V* map_find(DynamicMap<K, V[n]> *map, const K &key)
{
    i32 i = map_find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value[0];
}

template<typename K, typename V>
V* map_find_emplace(DynamicMap<K, V> *map, const K &key)
{
    i32 slot = map_find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        slot = map_set_slot(map, slot, key, V{});
    }

    return &map->slots[slot].value;
}

template<typename K, typename V, i32 n>
V* map_find_emplace(DynamicMap<K, V[n]> *map, const K &key)
{
    i32 slot = map_find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        V value[n] = {};
        slot = map_set_slot(map, slot, key, value);
    }

    return &map->slots[slot].value[0];
}

template<typename K, typename V>
V* map_find_emplace(DynamicMap<K, V> *map, const K &key, const V &emp_value)
{
    i32 slot = map_find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        slot = map_set_slot(map, slot, key, emp_value);
    }

    return &map->slots[slot].value;
}


// string specializations
template<typename V>
void map_set(DynamicMap<String, V> *map, const char *key, V value) { return map_set(map, string(key), value); }

template<typename V>
V* map_find(DynamicMap<String, V> *map, String key)
{
    i32 i = map_find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value;
}

// -- test suite
#include "test.h"

#ifdef DO_TESTS
static TEST_PROC(dynamic_map__set_invokes_copy_constructor_for_key_and_value)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });

    ASSERT(TestType::copy_constructor_calls == 6);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 0);
    ASSERT(TestType::move_assignment_calls == 0);
}

static TEST_PROC(dynamic_map__growing_map_invokes_copy_constructors)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });

    void *old = map.slots;
    map_grow(&map, map.capacity+2);
    ASSERT(map.slots != old);

    ASSERT(TestType::copy_constructor_calls == 12);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 0);
    ASSERT(TestType::move_assignment_calls == 0);
}

static TEST_PROC(dynamic_map__set_of_existing_key_invokes_copy_assign_for_value_and_nothing_for_key)
{
    TestType::reset_counters();
    DynamicMap<TestType, TestType> map{};

    map_set(&map, { 1 }, { 1 });
    map_set(&map, { 2 }, { 2 });
    map_set(&map, { 3 }, { 3 });
    map_set(&map, { 2 }, { 1 });

    ASSERT(TestType::copy_constructor_calls == 6);
    ASSERT(TestType::move_constructor_calls == 0);
    ASSERT(TestType::copy_assignment_calls == 1);
    ASSERT(TestType::move_assignment_calls == 0);
}
#endif // DO-TESTS

#endif // MAP_H

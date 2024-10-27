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
        if (map->slots[i].occupied) {
            FREE(map->alloc, map->slots[i].key.data);
        }
    }

    map->slots = nullptr;
    map->count = 0;
    map->capacity = 0;
}

template<typename K, typename V>
void map_set(DynamicMap<K, V> *map, K key, V value);

template<typename K, typename V>
i32 find_slot(DynamicMap<K, V> *map, K key)
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
void grow_map(DynamicMap<K, V> *map, i32 new_capacity)
{
    ASSERT(new_capacity > map->capacity);
    if (map->alloc.proc == nullptr) map->alloc = mem_dynamic;

    using Pair = typename DynamicMap<K,V>::Pair;
    DynamicMap<K, V> old_table = *map;

    map->slots = ALLOC_ARR(map->alloc, Pair, new_capacity);
    for (i32 i = 0; i < new_capacity; i++) map->slots[i].occupied = false;
    map->capacity = new_capacity;
    map->count = 0;

    for (i32 i = 0; i < old_table.capacity; i++) {
        if (old_table.slots[i].occupied) {
            map_set(map, old_table.slots[i].key, old_table.slots[i].value);
        }
    }

    if (old_table.slots) FREE(map->alloc, old_table.slots);
}

template<typename K, typename V>
i32 set_slot(DynamicMap<K, V> *map, i32 slot, K key, V value)
{
    if (map->count >= map->capacity*DYNAMIC_MAP_LOAD_FACTOR) {
        i32 new_capacity = map->capacity == 0 ? DYNAMIC_MAP_INNITIAL_CAPACITY : map->capacity*2;
        grow_map(map, new_capacity);

        slot = find_slot(map, key);
        ASSERT(slot >= 0);
    }

    map->slots[slot] = { .key = key, .value = value, .occupied = true };
    map->count++;

    return slot;
}

template<typename K, typename V, i32 n>
i32 set_slot(DynamicMap<K, V[n]> *map, i32 slot, K key, V value[n])
{
    if (map->count >= map->capacity*DYNAMIC_MAP_LOAD_FACTOR) {
        i32 new_capacity = map->capacity == 0 ? DYNAMIC_MAP_INNITIAL_CAPACITY : map->capacity*2;
        grow_map(map, new_capacity);

        slot = find_slot(map, key);
        ASSERT(slot >= 0);
    }

    map->slots[slot] = { .key = key, .occupied = true };
    for (i32 i = 0; i < n; i++) map->slots[slot].value[i] = value[i];
    map->count++;

    return slot;
}

template<typename K, typename V>
void map_set(DynamicMap<K, V> *map, K key, V value)
{
    i32 slot = find_slot(map, key);
    if (slot >= 0 && map->slots[slot].occupied) {
        map->slots[slot].value = value;
        return;
    }

    set_slot(map, slot, key, value);
}

template<typename K, typename V, i32 n>
void map_set(DynamicMap<K, V[n]> *map, K key, V value[n])
{
    i32 slot = find_slot(map, key);
    if (slot >= 0 && map->slots[slot].occupied) {
        for (i32 i = 0; i < n; i++) map->slots[slot].value[i] = value[i];
        return;
    }

    set_slot(map, slot, key, value);
}

template<typename K, typename V>
void map_remove(DynamicMap<K, V> *map, K key)
{
    i32 slot = find_slot(map, key);
    if (slot == -1 || !map->slots[slot].occupied) return;
    map->slots[slot].occupied = false;
    map->count--;
}

template<typename K, typename V>
V* map_find(DynamicMap<K, V> *map, K key)
{
    i32 i = find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value;
}

template<typename K, typename V, i32 n>
V* map_find(DynamicMap<K, V[n]> *map, K key)
{
    i32 i = find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value[0];
}

template<typename K, typename V>
V* map_find_emplace(DynamicMap<K, V> *map, K key)
{
    i32 slot = find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        slot = set_slot(map, slot, key, V{});
    }

    return &map->slots[slot].value;
}

template<typename K, typename V, i32 n>
V* map_find_emplace(DynamicMap<K, V[n]> *map, K key)
{
    i32 slot = find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        V value[n] = {};
        slot = set_slot(map, slot, key, value);
    }

    return &map->slots[slot].value[0];
}

template<typename K, typename V>
V* map_find_emplace(DynamicMap<K, V> *map, K key, V emp_value)
{
    i32 slot = find_slot(map, key);

    if (slot == -1 || !map->slots[slot].occupied) {
        slot = set_slot(map, slot, key, emp_value);
    }

    return &map->slots[slot].value;
}


// string specializations
template<typename V>
void map_set(DynamicMap<String, V> *map, const char *key, V value) { return map_set(map, string(key), value); }

template<typename V>
V* map_find(DynamicMap<String, V> *map, String key)
{
    i32 i = find_slot(map, key);
    if (i == -1 || !map->slots[i].occupied) return nullptr;
    return &map->slots[i].value;
}

template<typename V>
void map_find(DynamicMap<String, V> *map, String key, V value)
{
    i32 i = find_slot(map, key);
    if (i >= 0 && map->slots[i].occupied) {
        map->slots[i].value = value;
        return;
    }

    if (map->count >= map->capacity*DYNAMIC_MAP_LOAD_FACTOR) {
        i32 new_capacity = map->capacity == 0 ? DYNAMIC_MAP_INNITIAL_CAPACITY : map->capacity*2;
        grow_map(map, new_capacity);
    }

    i = find_slot(map, key);
    ASSERT(i >= 0);

    map->slots[i] = { .key = duplicate_string(key, map->alloc), .value = value, .occupied = true };
    map->count++;
}

#endif // MAP_H

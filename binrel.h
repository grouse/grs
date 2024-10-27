#ifndef BINREL_H
#define BINREL_H

#include "memory.h"
#include "map.h"

template<typename T0, typename T1>
struct OneToMany {
    Allocator alloc = mem_dynamic;

    DynamicMap<T0, DynamicArray<T1>> left;
    DynamicMap<T1, T0> right;
};

template<typename T0, typename T1>
void rel_reset(OneToMany<T0, T1> *rel)
{
    for (auto &it : rel->left) array_reset(&it.value);
    map_reset(&rel->left);
    map_reset(&rel->right);
}

template<typename T0, typename T1>
void rel_add(OneToMany<T0, T1> *rel, T0 left, T1 right)
{
    if (auto *old = map_find(&rel->right, right); old) {
        auto *old_arr = map_find(&rel->left, *old);

        i32 index = array_find_index(*old_arr, right);
        if (index >= 0) array_remove(old_arr, index);
        map_remove(&rel->right, right);
    }

    auto *arr = map_find_emplace(&rel->left, left, { .alloc = rel->alloc });
    array_add(arr, right);
    map_set(&rel->right, right, left);
}

template<typename T0, typename T1>
T0* rel_left(OneToMany<T0, T1> *rel, T1 right)
{
    return map_find(&rel->right, right);
}

template<typename T0, typename T1>
Array<T1> rel_right(OneToMany<T0, T1> *rel, T0 left)
{
    DynamicArray<T1> *arr = map_find(&rel->left, left);
    return arr ? *arr : Array<T1>{};
}

#endif // BINREL_H

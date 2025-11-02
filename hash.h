#ifndef HASH_H
#define HASH_H

#include "core.h"
#include "string.h"

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

#define HASH32_SEED ((u32)0)

inline u32 hash32(const void *data, i32 size, u32 seed = HASH32_SEED)
{
    XXH32_hash_t val = XXH32(data, size, (XXH32_hash_t)seed);
    return (u32)val;
}


inline u32 hash32(i32 value, u32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline u32 hash32(i64 value, u32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}


inline u32 hash32(u32 value, u32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline u32 hash32(u64 value, u32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}


inline u32 hash32(f32 value, u32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}


inline u32 hash32(String str, u32 seed = HASH32_SEED)
{
    return hash32(str.data, str.length, seed);
}


inline u32 hash32(const void *ptr, u32 seed = HASH32_SEED)
{
    return hash32(&ptr, sizeof ptr, seed);
}

#endif // HASH_H

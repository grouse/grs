#ifndef HASH_H
#define HASH_H

#include "core.h"
#include "string.h"

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

typedef XXH32_hash_t h32;
typedef XXH64_hash_t h64;
typedef XXH128_hash_t h128;

typedef XXH32_state_t h32s;
typedef XXH3_state_t h64s;
typedef XXH3_state_t h128s;

inline bool operator==(const h128 &lhs, const h128 &rhs)
{
    return XXH128_isEqual(lhs, rhs);
}

#define HASH32_SEED ((h32)0)
#define HASH64_SEED ((h64)0)




inline h32 hash32(const void *data, i32 size, h32 seed = HASH32_SEED)
{
    XXH32_hash_t val = XXH32(data, size, (XXH32_hash_t)seed);
    return (h32)val;
}

inline h32s hash32_start(h32 seed = HASH32_SEED)
{
    XXH32_state_t state;
    XXH32_reset(&state, seed);
    return state;
}

inline void hash32_update(h32s *state, const void *data, i32 size)
{
    XXH32_update(state, data, size);
}

inline h32 hash32_digest(h32s *state)
{
    return XXH32_digest(state);
}


inline h64 hash64(const void *data, i32 size, h64 seed = HASH64_SEED)
{
    return XXH3_64bits_withSeed(data, size, seed);
}

inline h64s hash64_start(h64 seed = HASH64_SEED)
{
    XXH3_state_t state;
    XXH3_INITSTATE(&state);
    XXH3_64bits_reset_withSeed(&state, seed);
    return state;
}

inline void hash64_update(h64s *state, const void *data, i32 size)
{
    XXH3_64bits_update(state, data, size);
}

inline h64 hash64_digest(h64s *state)
{
    return XXH3_64bits_digest(state);
}


inline h128 hash128(const void *data, i32 size, h64 seed = HASH64_SEED)
{
    return XXH3_128bits_withSeed(data, size, seed);
}

inline h128s hash128_start(h64 seed = HASH64_SEED)
{
    XXH3_state_t state;
    XXH3_INITSTATE(&state);
    XXH3_128bits_reset_withSeed(&state, seed);
    return state;
}

inline void hash128_update(h128s *state, const void *data, i32 size)
{
    XXH3_128bits_update(state, data, size);
}

inline h128 hash128_digest(h128s *state)
{
    return XXH3_128bits_digest(state);
}


inline h32 hash32(i16 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(i32 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(i64 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(u8 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(u16 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(u32 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(u64 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(f32 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(f64 value, h32 seed = HASH32_SEED)
{
    return hash32(&value, sizeof value, seed);
}

inline h32 hash32(String str, h32 seed = HASH32_SEED)
{
    return hash32(str.data, str.length, seed);
}


inline h64 hash64(i16 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(i32 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(i64 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(u16 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(u32 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(u64 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(f32 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(f64 value, h64 seed = HASH64_SEED)
{
    return hash64(&value, sizeof value, seed);
}

inline h64 hash64(String str, h64 seed = HASH64_SEED)
{
    return hash64(str.data, str.length, seed);
}



inline h128 hash128(i16 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(i32 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(i64 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(u8 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(u16 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(u32 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(u64 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(f32 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(f64 value, h64 seed = HASH64_SEED)
{
    return hash128(&value, sizeof value, seed);
}

inline h128 hash128(String str, h64 seed = HASH64_SEED)
{
    return hash128(str.data, str.length, seed);
}

#endif // HASH_H

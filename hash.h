#ifndef HASH_H
#define HASH_H

#include "core.h"
#include "string.h"

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

#include <type_traits>

typedef XXH32_hash_t h32;
typedef XXH64_hash_t h64;
typedef XXH128_hash_t h128;

typedef XXH32_state_t h32s;
typedef XXH3_state_t h64s;
typedef XXH3_state_t h128s;

#define HASH32_SEED ((h32)0)
#define HASH64_SEED ((h64)0)

#define HASH32_DECL(T, state, var)\
    void hash32_update(h32s *state, const T &var);\
    inline h32 hash32(const T &var, h32 seed = HASH32_SEED)\
    {\
        h32s state = hash32_start(seed);\
        hash32_update(&state, var);\
        return hash32_digest(&state);\
    }\
    inline void hash32_update(h32s *state, const T &var)\

#define HASH64_DECL(T, state, var)\
    void hash64_update(h64s *state, const T &var);\
    inline h64 hash64(const T &var, h64 seed = HASH64_SEED)\
    {\
        h64s state = hash64_start(seed);\
        hash64_update(&state, var);\
        return hash64_digest(&state);\
    }\
    inline void hash64_update(h64s *state, const T &var)\

#define HASH128_DECL(T, state, var)\
    void hash128_update(h128s *state, const T &var);\
    inline h128 hash128(const T &var, h64 seed = HASH64_SEED)\
    {\
        h128s state = hash128_start(seed);\
        hash128_update(&state, var);\
        return hash128_digest(&state);\
    }\
    inline void hash128_update(h128s *state, const T &var)\


template<typename T>
concept ByteHashable = 
    std::is_trivially_copyable_v<T> &&
    std::has_unique_object_representations_v<T>;


inline bool operator==(const h128 &lhs, const h128 &rhs)
{
    return XXH128_isEqual(lhs, rhs);
}

// NOTE(jesper): results in unexpected overload resolution for cases like hash32(ptr, seed) if ptr does not implicitly cast to void*, in which case it takes the void* prim overload declared below
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

template<ByteHashable T>
void hash32_update(h32s *state, const T& value)
{
    hash32_update(state, &value, sizeof value);
}

template<ByteHashable T>
h32 hash32(const T& value, h32 seed = HASH32_SEED)
{
    h32s state = hash32_start(seed);
    hash32_update(&state, value);
    return hash32_digest(&state);
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

template<ByteHashable T>
void hash64_update(h64s *state, const T& value)
{
    hash64_update(state, &value, sizeof value);
}

template<ByteHashable T>
h64 hash64(const T& value, h64 seed = HASH64_SEED)
{
    h64s state = hash64_start(seed);
    hash64_update(&state, value);
    return hash64_digest(&state);
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

template<ByteHashable T>
void hash128_update(h128s *state, const T& value)
{
    hash128_update(state, &value, sizeof value);
}

template<ByteHashable T>
h128 hash128(const T& value, h64 seed = HASH64_SEED)
{
    h128s state = hash128_start(seed);
    hash128_update(&state, value);
    return hash128_digest(&state);
}

HASH32_DECL(String, state, str) { hash32_update(state, str.data, str.length); }
HASH64_DECL(String, state, str) { hash64_update(state, str.data, str.length); }
HASH128_DECL(String, state, str) { hash128_update(state, str.data, str.length); }

// NOTE(jesper): this will produce different hashes for e.g. +0.0f and -0.0f
HASH32_DECL(f32, state, value) { hash32_update(state, &value, sizeof value); }
HASH64_DECL(f32, state, value) { hash64_update(state, &value, sizeof value); }
HASH128_DECL(f32, state, value) { hash128_update(state, &value, sizeof value); }

#endif // HASH_H

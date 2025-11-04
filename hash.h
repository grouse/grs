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


#define HASH32_DECL_PRIM(T)\
    inline h32 hash32(const T value, h32 seed = HASH32_SEED)\
    {\
        return hash32(&value, sizeof value, seed);\
    }\
    inline void hash32_update(h32s *state, const T value)\
    {\
        hash32_update(state, &value, sizeof value);\
    }

#define HASH64_DECL_PRIM(T)\
    inline h64 hash64(const T value, h64 seed = HASH64_SEED)\
    {\
        return hash64(&value, sizeof value, seed);\
    }\
    inline void hash64_update(h64s *state, const T value)\
    {\
        hash64_update(state, &value, sizeof value);\
    }

#define HASH128_DECL_PRIM(T)\
    inline h128 hash128(const T value, h64 seed = HASH64_SEED)\
    {\
        return hash128(&value, sizeof value, seed);\
    }\
    inline void hash128_update(h128s *state, const T value)\
    {\
        hash128_update(state, &value, sizeof value);\
    }

#define HASH_DECL_PRIM(T)\
    HASH32_DECL_PRIM(T)\
    HASH64_DECL_PRIM(T)\
    HASH128_DECL_PRIM(T)

#define HASH32_DECL(T, state, var)\
void hash32_update(h32s *state, const T &var);\
inline u32 hash32(const T &var, u32 seed = HASH32_SEED)\
{\
    h32s state = hash32_start(seed);\
    hash32_update(&state, var);\
    return hash32_digest(&state);\
}\
inline void hash32_update(h32s *state, const T &var)\


HASH_DECL_PRIM(i8);
HASH_DECL_PRIM(i16);
HASH_DECL_PRIM(i32);
HASH_DECL_PRIM(i64);

HASH_DECL_PRIM(u8);
HASH_DECL_PRIM(u16);
HASH_DECL_PRIM(u32);
HASH_DECL_PRIM(u64);

HASH_DECL_PRIM(f32);
HASH_DECL_PRIM(f64);

// NOTE(jesper): I don't like having this here. it hashes the pointer address, not the contents, but currently mandatory to not completely break on hash32((void*)ptr, seed) cases
HASH_DECL_PRIM(void*);


inline h32 hash32(const String str, h32 seed = HASH32_SEED)
{
    return hash32(str.data, str.length, seed);
}
inline void hash32_update(h32s *state, const String str)
{
    return hash32_update(state, str.data, str.length);
}

inline h64 hash64(const String str, h64 seed = HASH64_SEED)
{
    return hash64(str.data, str.length, seed);
}
inline void hash64_update(h64s *state, const String str)
{
    return hash64_update(state, str.data, str.length);
}

inline h128 hash128(const String str, h64 seed = HASH64_SEED)
{
    return hash128(str.data, str.length, seed);
}
inline void hash128_update(h128s *state, const String str)
{
    return hash128_update(state, str.data, str.length);
}


#endif // HASH_H

#ifndef ASSETS_H
#define ASSETS_H

#include "platform.h"
#include "string.h"
#include "array.h"

struct Asset;

struct AssetHandle {
    i32 index;
    i32 gen;

    bool operator!=(const AssetHandle &rhs) const { return index != rhs.index || gen != rhs.gen; }
    bool operator==(const AssetHandle &rhs) const { return index == rhs.index && gen == rhs.gen; }

    explicit operator bool() const { return index != 0 || gen != 0; }
};

constexpr AssetHandle ASSET_HANDLE_INVALID = { 0, 0 };

typedef void* (*asset_load_t)(AssetHandle handle, void *existing, String identifier, u8 *data, i32 size);
typedef bool  (*asset_save_t)(AssetHandle handle, StringBuilder *stream, void *data);

#define ASSET_LOAD_PROC(name) void* name(AssetHandle handle, void *existing, String identifier, u8 *data, i32 size)

struct AssetTypesDesc {
    struct {
        String ext;
        i32 type_id;
        asset_load_t load_proc;
        asset_save_t save_proc;
    } types[10];
};

struct Asset {
    i32 gen;

    u64 last_saved;
    u64 last_modified;

    String path;
    String identifier;

    i32 type_id;
    void *data;
};

#include "generated/assets.h"

template<typename T>
T* find_asset(String path)
{
    Asset *asset = find_asset_by_path(path);
    if (!asset) return nullptr;
    if (asset->type_id != typeid(T)) {
        LOG_ERROR("mismatching asset type id for asset: '%.*s', got %d expected %d", STRFMT(asset->path), typeid(T), asset->type_id);
        return nullptr;
    }

    return (T*)asset->data;
}

template<typename T>
T* get_asset(AssetHandle handle)
{
    Asset *asset = get_asset(handle);
    if (!asset) return nullptr;
    if (asset->type_id != typeid(T)) {
        LOG_ERROR("mismatching asset type id for asset: '%.*s', got %d expected %d", STRFMT(asset->path), typeid(T), asset->type_id);
        return nullptr;
    }
    return (T*)asset->data;
}

template<typename T>
T* get_asset_data(AssetHandle handle)
{
    Asset *asset = get_asset(handle);
    if (!asset) return nullptr;
    return (T*)asset->data;
}

#endif // ASSETS_H

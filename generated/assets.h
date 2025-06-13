#ifndef ASSETS_PUBLIC_H
#define ASSETS_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern void init_assets(Array<String> folders, const AssetTypesDesc & desc);
extern void register_asset_procs(const AssetTypesDesc & desc);
extern bool is_asset_loaded(String path);
extern AssetHandle find_loaded_asset(String path);
extern AssetHandle find_asset_handle(String path);
extern Asset *find_asset_by_path(String path);
extern i32 get_asset_type(AssetHandle handle);
extern Asset *get_asset(AssetHandle handle);
extern String get_asset_path(AssetHandle handle);
extern String get_asset_identifier(AssetHandle handle);
extern AssetHandle gen_asset_handle();
extern AssetHandle create_asset(AssetHandle handle, Asset asset);
extern AssetHandle create_asset(Asset asset);
extern AssetHandle create_asset(AssetHandle handle, String path, i32 type_id, void *data);
extern AssetHandle create_asset(String path, i32 type_id, void *data);
extern AssetHandle restore_removed_asset(String path);
extern void restore_removed_asset(AssetHandle handle);
extern void remove_asset(AssetHandle handle);
extern bool asset_path_used(String path);
extern bool load_asset(AssetHandle handle, u8 *contents, i32 size);
extern AssetHandle load_asset(String path);
extern AssetHandle load_asset(String path, u8 *contents, i32 size);
extern bool ensure_loaded(AssetHandle handle);
extern void dirty_asset(AssetHandle handle);
extern void save_dirty_assets();
extern Array<AssetHandle> get_unsaved_assets(Allocator mem);
extern void save_asset(AssetHandle handle);
extern String resolve_asset_path(String path, Allocator mem);
extern String normalise_asset_path(String path, Allocator mem);
extern Array<String> list_asset_files(Allocator mem);
extern Array<String> list_asset_files(i32 type, Allocator mem);
extern Array<String> list_asset_files(Array<String> extensions, Allocator mem);
extern void *load_string_asset(AssetHandle, void *existing, String, u8 *data, i32 size);
extern u32 hash32(const AssetHandle & it, u32 seed = MURMUR3_SEED);

#endif // ASSETS_PUBLIC_H

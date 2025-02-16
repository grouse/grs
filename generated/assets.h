#ifndef ASSETS_PUBLIC_H
#define ASSETS_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern i32 get_asset_type(AssetHandle handle);
extern AssetHandle create_asset(AssetHandle handle, Asset asset);
extern AssetHandle create_asset(Asset asset);
extern AssetHandle create_asset(AssetHandle handle, String path, i32 type_id, void *data);
extern AssetHandle create_asset(String path, i32 type_id, void *data);
extern Array<String> list_asset_files(i32 type, Allocator mem);

#endif // ASSETS_PUBLIC_H

#include "assets.h"
#include "file.h"
#include "lexer.h"
#include "map.h"
#include "core.h"

#include "stb/stb_image.h"

i32 next_asset_type_id = 0;

struct {
    DynamicArray<String> folders;

    DynamicArray<Asset> loaded;
    DynamicArray<AssetHandle> removed;
    DynamicArray<AssetHandle> free_slots;

    DynamicMap<String, i32> types;
    DynamicMap<String, asset_load_t> load_procs;
    DynamicMap<String, asset_save_t> save_procs;
} assets{};


void init_assets(Array<String> folders, const AssetTypesDesc &desc) EXPORT
{
    SArena scratch = tl_scratch_arena();

    assets.folders = { .alloc = mem_dynamic };
    array_reserve(&assets.folders, folders.count);
    for (auto it : folders) {
        String path = absolute_path(it, scratch);

        if (!path) continue;
        if (array_contains(assets.folders, path)) continue;

        LOG_INFO("adding asset folder: %.*s", STRFMT(path));
        array_add(&assets.folders, duplicate_string(path, mem_dynamic));
    }

    assets.load_procs.alloc = mem_dynamic;
    assets.save_procs.alloc = mem_dynamic;

    for (i32 i = 0; i < ARRAY_COUNT(desc.types); i++) {
        if (desc.types[i].ext.length == 0) break;
        map_set(&assets.types, desc.types[i].ext, desc.types[i].type_id);
        map_set(&assets.load_procs, desc.types[i].ext, desc.types[i].load_proc);
        map_set(&assets.save_procs, desc.types[i].ext, desc.types[i].save_proc);
    }
}

bool is_asset_loaded(String path) EXPORT
{
    for (auto it : assets.loaded) if (it.path == path) return true;
    return false;
}

AssetHandle find_loaded_asset(String path) EXPORT
{
    for (i32 i = 0; i < assets.loaded.count; i++) {
        if (assets.loaded[i].path == path) {
            return AssetHandle{ .index = i, .gen = assets.loaded[i].gen };
        }
    }

    return ASSET_HANDLE_INVALID;
}

AssetHandle find_asset_handle(String path) EXPORT
{
    SArena scratch = tl_scratch_arena();
    String apath = resolve_asset_path(path, scratch);

    if (!apath.length) return ASSET_HANDLE_INVALID;

    for (i32 i = 0; i < assets.loaded.count; i++) {
        if (path_equals(assets.loaded[i].path, apath)) {
            return AssetHandle{ .index = i, .gen = assets.loaded[i].gen };
        }
    }

    String ext = extension_of(apath);
    i32 *type = map_find(&assets.types, ext);
    if (!type) LOG_ERROR("unknown asset type for asset '%.*s' with ext: '%.*s'", STRFMT(apath), STRFMT(ext));

    return create_asset(apath, *type, nullptr);
}

Asset* find_asset_by_path(String path) EXPORT
{
    AssetHandle handle = find_asset_handle(path);
    if (handle == ASSET_HANDLE_INVALID) return nullptr;
    return get_asset(handle);
}

i32 get_asset_type(AssetHandle handle) EXPORT
{
    ASSERT(handle != ASSET_HANDLE_INVALID);
    return assets.loaded[handle.index].type_id;
}

Asset* get_asset(AssetHandle handle) EXPORT
{
    SArena scratch = tl_scratch_arena();
    ASSERT(handle != ASSET_HANDLE_INVALID);

    Asset asset = assets.loaded[handle.index];
    if (asset.gen != handle.gen) return nullptr;

    if (!asset.data) {
        FileInfo fi = read_file(asset.path, scratch);
        if (!fi.data) {
            LOG_ERROR("unable to load asset '%.*s'", STRFMT(asset.path));
            return nullptr;
        }

        if (!load_asset(handle, fi.data, fi.size)) return nullptr;
    }

    return &assets.loaded[handle.index];
}

String get_asset_path(AssetHandle handle) EXPORT
{
    ASSERT(handle != ASSET_HANDLE_INVALID);

    Asset &asset = assets.loaded[handle.index];
    if (asset.gen != handle.gen) return {};
    return asset.path;
}

AssetHandle gen_asset_handle() EXPORT
{
    if (assets.free_slots.count > 0) return array_pop(&assets.free_slots);
    i32 i = array_add(&assets.loaded, { .gen = 1 });
    AssetHandle handle{ .index = i, .gen = 1 };

    //LOG_INFO("generated asset handle: { %d %d }", handle.index, handle.gen);
    return handle;
}

AssetHandle create_asset(AssetHandle handle, Asset asset) EXPORT
{
    //LOG_INFO("creating asset '%.*s', handle: { %d %d }", STRFMT(asset.path), handle.index, handle.gen);

    asset.identifier = filename_of(asset.path);

    if (assets.loaded.count <= handle.index) {
        ASSERT(handle.gen == 1);
        asset.gen = 1;
        array_add(&assets.loaded, asset);
    } else {
        assets.loaded[handle.index] = asset;
        assets.loaded[handle.index].gen = handle.gen;
    }

    return handle;
}

AssetHandle create_asset(Asset asset) EXPORT
{
    AssetHandle handle = gen_asset_handle();
    return create_asset(handle, asset);
}

AssetHandle create_asset(AssetHandle handle, String path, i32 type_id, void *data) EXPORT
{
    Asset asset{
        .path = duplicate_string(path, mem_dynamic),
        .type_id = type_id,
        .data = data,
    };

    if (file_exists(path)) asset.last_saved = file_modified_timestamp(path);
    else asset.last_modified = wall_timestamp();

    return create_asset(handle, asset);
}

AssetHandle create_asset(String path, i32 type_id, void *data) EXPORT
{
    AssetHandle handle = gen_asset_handle();
    return create_asset(handle, path, type_id, data);
}

AssetHandle restore_removed_asset(String path) EXPORT
{
    for (i32 i = 0; i < assets.removed.count; i++) {
        auto &it = assets.loaded[assets.removed[i].index];
        if (it.path == path) {
            AssetHandle handle = assets.removed[i];
            array_remove_unsorted(&assets.removed, i);
            return handle;
        }
    }

    return ASSET_HANDLE_INVALID;
}

void restore_removed_asset(AssetHandle handle) EXPORT
{
    for (i32 i = 0; i < assets.removed.count; i++) {
        if (assets.removed[i] == handle) {
            array_remove_unsorted(&assets.removed, i);
            return;
        }
    }
}

void remove_asset(AssetHandle handle) EXPORT
{
    if (handle.gen != assets.loaded[handle.index].gen) {
        LOG_ERROR("asset generation id mismatch between handle and loaded asset");
        return;
    }

    // NOTE(jesper): actual removal and reuse of assets and asset handles is currently disabled because we rely on stable asset handles in the editor undo/redo history stack. This needs to be either re-thoughts, or perhaps turned into a flag that is disabled/enabled depending on whether the editor is enabled
    //array_add(&assets.free_slots, { handle.index, handle.gen+1 });
    array_add(&assets.removed, handle);
    //assets.loaded[handle.index].gen++;
}

bool asset_path_used(String path) EXPORT
{
    for (auto it : assets.loaded) if (it.path == path) return true;
    return false;
}

bool load_asset(AssetHandle handle, u8 *contents, i32 size) EXPORT
{
    String path = assets.loaded[handle.index].path;
    if (assets.loaded[handle.index].gen == handle.gen &&
        assets.loaded[handle.index].data &&
        assets.loaded[handle.index].last_saved == file_modified_timestamp(path))
    {
        LOG_INFO("asset '%.*s' is up to date", STRFMT(path));
        return true;
    }

    String ext = extension_of(path);
    asset_load_t *load_proc = map_find(&assets.load_procs, ext);

    if (load_proc == nullptr) {
        LOG_ERROR("unknown load proc for asset '%.*s' with ext: '%.*s'", STRFMT(path), STRFMT(ext));
        return false;
    }

    LOG_INFO("loading asset '%.*s', handle: { %d %d }", STRFMT(path), handle.index, handle.gen);

    Asset asset = assets.loaded[handle.index];
    if (void *data = (*load_proc)(handle, asset.data, asset.identifier, contents, size); data) {
        Asset *asset = &assets.loaded[handle.index];
        asset->data = data;
        asset->last_saved = file_modified_timestamp(path);
        asset->last_modified = 0;

        return true;
    }

    return false;
}

AssetHandle load_asset(String path) EXPORT
{
    SArena scratch = tl_scratch_arena();

    String ext = extension_of(path);
    asset_load_t *load_proc = map_find(&assets.load_procs, ext);

    if (load_proc == nullptr || *load_proc == nullptr) {
        LOG_ERROR("unknown load proc for asset '%.*s' with ext: '%.*s'", STRFMT(path), STRFMT(ext));
        return ASSET_HANDLE_INVALID;
    }

    String apath = resolve_asset_path(path, scratch);
    if (!apath) {
        LOG_ERROR("unable to resolve asset path: %.*s", STRFMT(path));
        return ASSET_HANDLE_INVALID;
    }

    FileInfo file = read_file(apath, mem_dynamic);
    return load_asset(apath, file.data, file.size);
}

AssetHandle load_asset(String path, u8 *contents, i32 size) EXPORT
{
    String ext = extension_of(path);
    asset_load_t *load_proc = map_find(&assets.load_procs, ext);

    if (load_proc == nullptr || *load_proc == nullptr) {
        LOG_ERROR("unknown load proc for asset '%.*s' with ext: '%.*s'", STRFMT(path), STRFMT(ext));
        return ASSET_HANDLE_INVALID;
    }

    AssetHandle handle = find_loaded_asset(path);
    if (handle == ASSET_HANDLE_INVALID) {
        i32 *type = map_find(&assets.types, ext);
        if (!type) LOG_ERROR("unknown asset type for asset '%.*s' with ext: '%.*s'", STRFMT(path), STRFMT(ext));

        handle = create_asset(path, *type, nullptr);
    }

    if (!load_asset(handle, contents, size)) return ASSET_HANDLE_INVALID;
    return handle;
}

bool ensure_loaded(AssetHandle handle) EXPORT
{
    if (handle.gen != assets.loaded[handle.index].gen) {
        LOG_ERROR("asset generation id mismatch between handle and loaded asset");
        return false;
    }

    if (!assets.loaded[handle.index].data) {
        SArena scratch = tl_scratch_arena();

        String path = assets.loaded[handle.index].path;
        FileInfo fi = read_file(path, scratch);

        if (!fi.data) {
            LOG_ERROR("unable to load asset '%.*s'", STRFMT(path));
            return false;
        }

        if (!load_asset(handle, fi.data, fi.size)) return false;
    }

    return true;
}

void dirty_asset(AssetHandle handle) EXPORT
{
    ASSERT(handle.gen == assets.loaded[handle.index].gen);
    assets.loaded[handle.index].last_modified = wall_timestamp();
}

void save_dirty_assets() EXPORT
{
    SArena scratch = tl_scratch_arena();

    for (auto handle : assets.removed) {
        auto &it = assets.loaded[handle.index];

        if (it.last_modified > it.last_saved) {
            LOG_INFO("removing file for deleted asset: %.*s", STRFMT(it.path));
            remove_file(it.path);
            it.last_saved = wall_timestamp();
            it.last_modified = 0;
        }
    }

    StringBuilder stream{ .alloc = scratch };
    for (auto it : iterator(assets.loaded)) {
        if (it->last_modified > it->last_saved) {
            reset_string_builder(&stream);

            asset_save_t *save_proc = map_find(&assets.save_procs, extension_of(it->path));
            if (save_proc == nullptr || *save_proc == nullptr) {
                LOG_ERROR("unable to save asset '%.*s', unknown save procedure", STRFMT(it->path));
                continue;
            }

            LOG_INFO("saving modified asset: %.*s", STRFMT(it->path));
            if ((*save_proc)(AssetHandle{ it.index, it->gen }, &stream, it->data)) {
                write_file(it->path, &stream);

                it->last_saved = file_modified_timestamp(it->path);
                it->last_modified = 0;
            }
        }
    }
}

Array<AssetHandle> get_unsaved_assets(Allocator mem) EXPORT
{
    DynamicArray<AssetHandle> result{ .alloc = mem };

    for (i32 i = 0; i < assets.loaded.count; i++) {
        if (assets.loaded[i].last_modified > assets.loaded[i].last_saved) {
            array_add(&result, { .index = i, .gen = assets.loaded[i].gen });
        }
    }

    return result;
}

void save_asset(AssetHandle handle) EXPORT
{
    SArena scratch = tl_scratch_arena();

    StringBuilder stream{ .alloc = scratch };
    Asset *it = get_asset(handle);

    asset_save_t *save_proc = map_find(&assets.save_procs, extension_of(it->path));
    if (save_proc == nullptr || *save_proc == nullptr) {
        LOG_ERROR("unable to save asset '%.*s', unknown save procedure", STRFMT(it->path));
        return;
    }

    if ((*save_proc)(handle, &stream, it->data)) {
        write_file(it->path, &stream);

        it->last_saved = file_modified_timestamp(it->path);
        it->last_modified = 0;
    }
}

String resolve_asset_path(String path, Allocator mem) EXPORT
{
    SArena scratch = tl_scratch_arena(mem);

    if (file_exists(path) && !is_directory(path)) {
        return absolute_path(path, mem);
    }

    for (auto f : assets.folders) {
        String s = join_path(f, path, scratch);
        if (file_exists(s) && !is_directory(s)) return absolute_path(s, mem);
    }

    return "";
}

String normalise_asset_path(String path, Allocator mem) EXPORT
{
    String short_path = resolve_asset_path(path, mem);

    for (auto f : assets.folders) {
        if (starts_with(path, f)) {
            String s = slice(path, f.length+1);
            if (s.length < short_path.length) short_path = s;
        }
    }

    for (i32 i = 0; i < short_path.length; i++) {
        if (short_path[i] == '\\') short_path[i] = '/';
    }

    return short_path;
}

Array<String> list_asset_files(Allocator mem) EXPORT
{
    DynamicArray<String> files{ .alloc = mem };

    for (auto f : assets.folders) {
        i32 c = files.count;
        list_files(&files, f, mem, FILE_LIST_ABSOLUTE | FILE_LIST_RECURSIVE);

        for (i32 i = c; i < files.count; i++) {
            if (map_find(&assets.types, extension_of(files[i]))) continue;
            array_remove_unsorted(&files, i--);
        }

        // NOTE(jesper): handle the case of an asset folder being a subfolder to another. This should probably be handled in a much better way to avoid a lot of re-iteration of the filesystem, nevermind the allocation of the resolved file paths
        for (i32 i = 0; i < c; i++) {
            for (i32 j = c; j < files.count; j++) {
                if (files[i] == files[j]) array_remove_unsorted(&files, j--);
            }
        }
    }

    return files;
}

Array<String> list_asset_files(i32 type, Allocator mem) EXPORT
{
    DynamicArray<String> files{ .alloc = mem };

    for (auto f : assets.folders) {
        i32 c = files.count;
        list_files(&files, f, mem, FILE_LIST_ABSOLUTE | FILE_LIST_RECURSIVE);

        // NOTE(jesper): dumb type filtering. The asset system probably ought to keep track of the filesystem better on its own, so these kinds of queries can be O(1), or close to it
        for (i32 i = c; i < files.count; i++) {
            String ext = extension_of(files[i]);
            i32 *t = map_find(&assets.types, ext);
            if (t && *t == type) continue;
            array_remove_unsorted(&files, i--);
        }

        // NOTE(jesper): handle the case of an asset folder being a subfolder to another. This should probably be handled in a much better way to avoid a lot of re-iteration of the filesystem, nevermind the allocation of the resolved file paths
        for (i32 i = 0; i < c; i++) {
            for (i32 j = c; j < files.count; j++) {
                if (files[i] == files[j]) array_remove_unsorted(&files, j--);
            }
        }
    }

    return files;
}

Array<String> list_asset_files(String ext, Allocator mem) EXPORT
{
    DynamicArray<String> files{ .alloc = mem };

    for (auto dir : assets.folders) {
        i32 c = files.count;
        list_files(&files, dir, ext, mem, FILE_LIST_ABSOLUTE | FILE_LIST_RECURSIVE);

        // NOTE(jesper): handle the case of an asset folder being a subfolder to another. This should probably be handled in a much better way to avoid a lot of re-iteration of the filesystem, nevermind the allocation of the resolved file paths
        for (i32 i = 0; i < c; i++) {
            for (i32 j = c; j < files.count; j++) {
                if (files[i] == files[j]) array_remove_unsorted(&files, j--);
            }
        }
    }

    return files;
}


void* load_string_asset(AssetHandle /*handle*/, void *existing, String /*identifier*/, u8 *data, i32 size) EXPORT
{
    if (existing) FREE(mem_dynamic, existing);
    return ALLOC_T(mem_dynamic, String) { (char*)data, size };
}

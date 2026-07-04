#include "AssetStore.hpp"

AssetStore::~AssetStore() {
    for (auto& [path, tex] : cache_) {
        UnloadTexture(tex);
    }
}

Texture2D AssetStore::load(const std::string& path) {
    auto it = cache_.find(path);
    if (it != cache_.end()) return it->second;

    Texture2D tex = LoadTexture(path.c_str());
    SetTextureFilter(tex, TEXTURE_FILTER_POINT); // crisp pixel art
    cache_[path] = tex;
    return tex;
}

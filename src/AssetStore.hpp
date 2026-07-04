#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

// Loads and owns textures, so Sprites/Maps can hold cheap non-owning handles.
// All textures are unloaded once, in the destructor.
class AssetStore {
public:
    ~AssetStore();
    Texture2D load(const std::string& path); // cached; nearest-neighbor filter

private:
    std::unordered_map<std::string, Texture2D> cache_;
};

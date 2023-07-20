#pragma once

#include <string>

enum class AssetType {
    Image,
    Sound
};

struct Asset {
    AssetType type;
    std::string path;
};

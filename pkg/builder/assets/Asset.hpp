#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <variant>

enum class AssetType {
    Image,
    Sound
};

struct Asset {
    AssetType type;
    std::string path;
};

#pragma once

#include <string>

namespace assets {
enum class Type {
    Image,
    Sound
};

struct Asset {
    Type type;
    std::string path;
};
}  // namespace assets

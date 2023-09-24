#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include "Errors.hpp"

namespace nl = nlohmann;

namespace sf {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2f, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2i, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2u, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2<std::string>, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FloatRect, left, top, width, height);
}  // namespace sf

namespace json {
inline ErrorOr<nl::json> read(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        return Error::New(fmt::format("No such file: {}", path));
    }

    nl::json res;

    try {
        file >> res;
        return res;
    } catch (nl::json::exception& e) {
        return Error::New(e.what());
        // TODO: return different errors?
    }
}

inline void create(const std::filesystem::path& path, const nl::json& json = "{}"_json) {
    std::ofstream file(path);
    file << std::setw(4) << json;
}

template <typename T>
T get(const nl::json& json, const std::string& key, const T& default_value = T{}) {
    try {
        return json.at(key).template get<T>();
    } catch (...) {
        return default_value;
    }
}
}  // namespace json

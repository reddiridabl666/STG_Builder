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

namespace nlohmann {
template <typename T>
struct adl_serializer<std::unique_ptr<T>> {
    static void to_json(json& j, const std::unique_ptr<T>& ptr) {
        if (ptr) {
            j = *ptr;
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, std::unique_ptr<T>& ptr) {
        if (j.is_null()) {
            ptr = nullptr;
        } else {
            ptr = std::make_unique<T>(j.get<T>());
        }
    }
};
}  // namespace nlohmann

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
    }
}

inline void create(const std::filesystem::path& path, const nl::json& json = "{}"_json) {
    std::ofstream file(path);
    file << std::setw(4) << json;
}
}  // namespace json

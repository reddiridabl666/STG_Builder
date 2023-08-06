#pragma once

#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include "Errors.hpp"

namespace nl = nlohmann;

namespace sf {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2f, x, y);
}  // namespace sf

namespace json {
inline ErrorOr<nl::json> read(const std::string& path) {
    std::ifstream file(path);
    nl::json res;

    try {
        file >> res;
        return res;
    } catch (nl::json::exception& e) {
        return unexpected_error<InternalError>(e.what());
        // TODO: return different errors?
    }
}
}  // namespace json
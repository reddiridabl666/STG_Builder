#pragma once

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include "Errors.hpp"

namespace nl = nlohmann;

namespace json {
template <typename T>
tl::expected<T, ErrorPtr> get(nl::json value, std::string_view key) {
    try {
        return value.at(key).template get<T>();
    } catch (nl::json::exception& e) {
        return unexpected_error<InternalError>(e.what());
    }
}

}  // namespace json

namespace sf {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2f, x, y);
}  // namespace sf

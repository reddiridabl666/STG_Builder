#pragma once

#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include "Errors.hpp"

namespace json {
template <typename T>
tl::expected<T, InternalError> get(nlohmann::json value, std::string_view key) {
    try {
        return value.at(key).template get<T>();
    } catch (nlohmann::json::exception& e) {
        return tl::unexpected(InternalError(e.what()));
    }
}

}  // namespace json

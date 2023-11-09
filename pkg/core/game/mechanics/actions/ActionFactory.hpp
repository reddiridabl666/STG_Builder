#pragma once

#include <memory>

#include "Action.hpp"
#include "Json.hpp"

namespace action {
struct Factory {
    template <typename ActionType>
    static std::unique_ptr<ActionType> create(const nl::json&);
};
}  // namespace action

#pragma once

#include <memory>

#include "Action.hpp"
#include "Json.hpp"

namespace action {
struct Factory {
    static std::unique_ptr<Action> create(const nl::json&);
};
}  // namespace action

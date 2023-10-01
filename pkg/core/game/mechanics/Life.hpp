#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Json.hpp"

class GameObject;
class GameField;

namespace alive {
using update = std::function<bool(const GameObject&, const GameField&)>;

update timed(float seconds);

update in_bounds(float margin = 0);

const inline update default_func = in_bounds(0);

inline const update always = [](auto&, auto&) {
    return true;
};

inline const char* const types[4] = {"default", "always", "in_bounds", "timed"};

inline const std::unordered_map<std::string, nl::json> args = {
    {"default", {{"margin", 50}}},
    {"always", nl::json::object()},
    {"in_bounds", {{"margin", 0}}},
    {"timed", {{"seconds", 0}}},
};
}  // namespace alive

#pragma once

#include <functional>
#include <string>

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
}  // namespace alive

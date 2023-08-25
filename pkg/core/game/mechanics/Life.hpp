#pragma once

#include <functional>

class GameObject;
class GameField;

namespace alive {
using update = std::function<bool(const GameObject&, const GameField&)>;

update timed(float seconds);

update in_bounds(float margin = 0);

inline const update always = [](auto&, auto&) {
    return true;
};
}  // namespace alive

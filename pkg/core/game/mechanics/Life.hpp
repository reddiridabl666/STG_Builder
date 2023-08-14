#pragma once

#include <functional>

class GameObject;
class GameField;

namespace life {
using update = std::function<bool(const GameObject&, const GameField&)>;

update timed(float seconds);

update in_bounds(float margin = 0);
}  // namespace life

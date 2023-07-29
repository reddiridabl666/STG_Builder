#pragma once

#include <functional>

class GameObject;

namespace movement {
using Func = std::function<void(GameObject& obj, float delta)>;

Func linear(float x = 0, float y = -1);

}  // namespace movement

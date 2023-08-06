#pragma once

#include <SFML/System/Vector2.hpp>
#include <functional>

class GameObject;

namespace movement {
using Func = std::function<void(GameObject& obj, float delta)>;

inline const Func no_op = [](GameObject&, float) {};

Func linear(float x = 0, float y = -1);

Func circular(sf::Vector2f center, float radius);  // find a way to pass obj here

Func user_control(int user_num = 1);

}  // namespace movement
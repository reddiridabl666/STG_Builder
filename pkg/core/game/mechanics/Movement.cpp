#include "Movement.hpp"

#include "GameObject.hpp"

namespace movement {
Func linear(float x, float y) {
    return [x, y](GameObject& obj, float delta) {
        auto speed = obj.speed();
        obj.move(speed * x * delta, speed * y * delta);
    };
}
}  // namespace movement

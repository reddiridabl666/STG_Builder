#include "Movement.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameObject.hpp"
#include "LinAlg.hpp"

using Key = sf::Keyboard;

namespace {
bool keys_are_pressed(const KeyList& keys) {
    for (auto key : keys) {
        if (Key::isKeyPressed(key)) {
            return true;
        }
    }
    return false;
}

int horizontal(const KeyControls& keys, const JoyControls&) {
    if (keys_are_pressed(keys.left)) {
        return -1;
    }

    if (keys_are_pressed(keys.right)) {
        return 1;
    }

    return 0;
}

int vertical(const KeyControls& keys, const JoyControls&) {
    if (keys_are_pressed(keys.up)) {
        return -1;
    }

    if (keys_are_pressed(keys.down)) {
        return 1;
    }

    return 0;
}
}  // namespace

namespace movement {
Func linear(float x, float y) {
    sf::Vector2f velocity;
    return Func(Func::Type::Pos, [velocity = sf::Vector2f{x, y}](const GameObject& obj, float delta_time) {
        return obj.pos() + obj.speed() * velocity * delta_time;
    });
}

Func circular(sf::Vector2f center, float speed) {
    return Func(Func::Type::Pos,
                [center, speed, angle = 0.f](const GameObject& obj, float delta_time) mutable {
                    angle += delta_time * speed;
                    static float radius = abs(center - obj.pos());

                    float x = center.x + radius * cos(angle);
                    float y = center.y + radius * sin(angle);

                    return sf::Vector2f{x, y};
                });
}

Func user_control(int user_num, const KeyControls& keys, const JoyControls& joy) {
    return Func(
        Func::Type::Velocity,
        [user_num, keys, joy](const GameObject&, float) {
            return sf::Vector2f{1.f * horizontal(keys, joy), 1.f * vertical(keys, joy)};
        },
        true);
}
}  // namespace movement

#include "Movement.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameObject.hpp"

namespace Key = sf::Keyboard;

namespace {
bool keys_are_pressed(std::initializer_list<Key::Key>&& keys) {
    for (auto key : keys) {
        if (Key::isKeyPressed(key)) {
            return true;
        }
    }
    return false;
}
}  // namespace

namespace movement {
Func linear(float x, float y) {
    return [x, y](GameObject& obj, float delta) {
        auto speed = obj.speed();
        obj.move(speed * x * delta, speed * y * delta);
    };
}

Func user_control(int user_num) {
    return [user_num](GameObject& obj, float delta) {
        if (keys_are_pressed({Key::W, Key::Up})) {
            obj.move(0, -1 * obj.speed() * delta);
        }

        if (keys_are_pressed({Key::S, Key::Down})) {
            obj.move(0, 1 * obj.speed() * delta);
        }

        if (keys_are_pressed({Key::A, Key::Left})) {
            obj.move(-1 * obj.speed() * delta, 0);
        }

        if (keys_are_pressed({Key::D, Key::Right})) {
            obj.move(1 * obj.speed() * delta, 0);
        }

        // TODO: remove hard-coded keys, add gamepad support
    };
}
}  // namespace movement

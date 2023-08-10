#include "Movement.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameObject.hpp"
#include "LinAlg.hpp"

using Key = sf::Keyboard;

namespace {
bool keys_are_pressed(const std::initializer_list<Key::Key>& keys) {
    for (auto key : keys) {
        if (Key::isKeyPressed(key)) {
            return true;
        }
    }
    return false;
}

// TODO: remove hard-coded keys, add gamepad support

int horizontal() {
    if (keys_are_pressed({Key::A, Key::Left})) {
        return -1;
    }

    if (keys_are_pressed({Key::D, Key::Right})) {
        return 1;
    }

    return 0;
}

int vertical() {
    if (keys_are_pressed({Key::W, Key::Up})) {
        return -1;
    }

    if (keys_are_pressed({Key::S, Key::Down})) {
        return 1;
    }

    return 0;
}
}  // namespace

namespace movement {
Func linear(float x, float y) {
    return Func(nullptr, [x, y](const GameObject&) {
        return sf::Vector2f{x, y};
    });
}

Func circular(sf::Vector2f center) {
    return Func([center](const GameObject& obj, float delta_time) {
        auto unit_vec = unit(center - obj.pos());
        return obj.velocity() + delta_time * unit_vec;
    });
}

Func user_control(int user_num) {
    return [user_num](const GameObject&, float) {
        return sf::Vector2f{1.f * horizontal(), 1.f * vertical()};
    };
}
}  // namespace movement

#pragma once

#include <SFML/System/Vector2.hpp>
#include <functional>
#include <string>

#include "Controls.hpp"

class GameObject;

namespace movement {

struct Func {
  public:
    enum class Type {
        Velocity,
        Pos,
        Unknown
    };

    Func() : type_(Type::Unknown), func_(nullptr) {}

    template <typename T>
    Func(Type type, T&& func, bool moves_with_field = false)
        : type_(type), func_(std::forward<T>(func)), moves_with_field_(moves_with_field) {}

    sf::Vector2f operator()(GameObject& obj, float delta) {
        return func_(obj, delta);
    }

    operator bool() const {
        return bool(func_);
    }

    Type type() const {
        return type_;
    }

    bool moves_with_field() const {
        return moves_with_field_;
    }

  private:
    using update = std::function<sf::Vector2f(const GameObject& obj, float delta)>;

    Type type_;
    update func_;

    bool moves_with_field_ = false;
};

inline const Func no_op;

Func linear(float x = 0, float y = -1);

Func circular(sf::Vector2f center, float);  // find a way to pass obj here

Func user_control(int user_num = 1, const KeyControls& keys = kDefaultKeyControls,
                  const JoyControls& joy = kDefaultJoyControls);

inline const char* const types[4] = {"none", "linear", "circular", "user_control"};

}  // namespace movement

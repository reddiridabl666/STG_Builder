#pragma once

#include <SFML/System/Vector2.hpp>
#include <concepts>
#include <functional>

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

    // template <typename T, typename U>
    // Func(T&& func, U&& initial) : func_(std::forward<T>(func)), initial_(std::forward<U>(initial)) {}

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

    // const update& velocity_func() const {
    //     return velocity_;
    // }

    // const update& pos_func() const {
    //     return pos_;
    // }

  private:
    using update = std::function<sf::Vector2f(const GameObject& obj, float delta)>;

    Type type_;
    update func_;

    bool moves_with_field_ = false;
    // update velocity_;
    // update pos_;
};

inline const Func no_op;

Func linear(float x = 0, float y = -1);

Func circular(sf::Vector2f center, float);  // find a way to pass obj here

Func user_control(int user_num = 1, const KeyControls& keys = kDefaultKeyControls,
                  const JoyControls& joy = kDefaultJoyControls);

}  // namespace movement

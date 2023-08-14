#pragma once

#include <SFML/System/Vector2.hpp>
#include <concepts>
#include <functional>

class GameObject;

namespace movement {
using update = std::function<sf::Vector2f(const GameObject& obj, float delta)>;

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
    Func(Type type, T&& func) : type_(type), func_(std::forward<T>(func)) {}

    sf::Vector2f operator()(GameObject& obj, float delta) {
        return func_(obj, delta);
    }

    operator bool() const {
        return bool(func_);
    }

    Type type() const {
        return type_;
    }

    // const update& velocity_func() const {
    //     return velocity_;
    // }

    // const update& pos_func() const {
    //     return pos_;
    // }

  private:
    Type type_;
    update func_;
    // update velocity_;
    // update pos_;
};

inline const Func no_op;

Func linear(float x = 0, float y = -1);

Func circular(sf::Vector2f center, float);  // find a way to pass obj here

Func user_control(int user_num = 1);

}  // namespace movement

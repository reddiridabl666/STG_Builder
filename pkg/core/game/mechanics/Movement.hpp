#pragma once

#include <SFML/System/Vector2.hpp>
#include <concepts>
#include <functional>

class GameObject;

namespace movement {
struct Func {
  private:
    using func_type = std::function<sf::Vector2f(const GameObject& obj, float delta)>;
    using initial_type = std::function<sf::Vector2f(const GameObject& obj)>;

  public:
    Func() = default;

    template <typename T, typename U>
    Func(T&& func, U&& initial) : func_(std::forward<T>(func)), initial_(std::forward<U>(initial)) {}

    template <typename T>
    Func(T&& func) : func_(std::forward<T>(func)) {}

    sf::Vector2f operator()(GameObject& obj, float delta) {
        return func_(obj, delta);
    }

    operator bool() const {
        return bool(func_);
    }

    const initial_type& initial() const {
        return initial_;
    }

  private:
    func_type func_;
    initial_type initial_;
};

inline const Func no_op;

Func linear(float x = 0, float y = -1);

Func circular(sf::Vector2f center);  // find a way to pass obj here

Func user_control(int user_num = 1);

}  // namespace movement

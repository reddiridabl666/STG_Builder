#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

template <typename T, typename U>
auto operator*(T val, const sf::Vector2<U>& vec) {
    return sf::Vector2<U>{val * vec.x, val * vec.y};
}

template <typename T, typename U>
auto operator*(const sf::Vector2<U>& vec, T val) {
    return val * vec;
}

template <typename T, typename U>
auto operator/(const sf::Vector2<U>& vec, T val) {
    return sf::Vector2<U>{vec.x / val, vec.x / val};
}

template <typename T, typename U>
auto operator/(T val, const sf::Vector2<U>& rhs) {
    return sf::Vector2<decltype(rhs.x / val)>{rhs.x / val, rhs.x / val};
}

template <typename T, typename U>
auto operator+(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return sf::Vector2<decltype(lhs.x + rhs.x)>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T, typename U>
auto operator-(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return sf::Vector2<decltype(lhs.x - rhs.x)>{lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T, typename U>
auto operator/(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return sf::Vector2<decltype(lhs.x / rhs.x)>{lhs.x / rhs.x, lhs.y / rhs.y};
}

template <typename T, typename U>
auto operator*(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return sf::Vector2<decltype(lhs.x * rhs.x)>{lhs.x * rhs.x, lhs.y * rhs.y};
}

template <typename T>
T abs(const sf::Vector2<T>& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

template <typename T>
sf::Vector2<T> unit(const sf::Vector2<T>& vec) {
    return vec / abs(vec);
}

template <typename T>
sf::Vector2<T> orthogonal_unit(const sf::Vector2<T>& vec) {
    T y = -1 * vec.x / vec.y;
    return unit(sf::Vector2<T>{1, y});
}

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
    return sf::Vector2<decltype(vec.x / val)>{vec.x / val, vec.y / val};
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

template <typename T, typename U>
auto distance(const sf::Vector2<T>& vec1, const sf::Vector2<U>& vec2) {
    return abs(vec1 - vec2);
}

template <typename T>
sf::Vector2<T> unit(const sf::Vector2<T>& vec) {
    return vec / abs(vec);
}

template <typename T>
sf::Vector2<T> orthogonal(const sf::Vector2<T>& vec) {
    return sf::Vector2<T>{vec.y, -1 * vec.x};
}

template <typename T>
sf::Vector2<T> orthogonal_unit(const sf::Vector2<T>& vec) {
    return unit(orthogonal(vec));
}

template <typename T, typename U>
auto dot(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T, typename U>
auto cos(const sf::Vector2<T>& lhs, const sf::Vector2<U>& rhs) {
    return dot(lhs, rhs) / (abs(lhs) * abs(rhs));
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vec) {
    return out << '{' << vec.x << ',' << vec.y << '}';
}

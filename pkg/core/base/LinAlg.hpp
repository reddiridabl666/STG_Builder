#pragma once

#include <SFML/System/Vector2.hpp>

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

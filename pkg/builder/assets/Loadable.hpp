#pragma once

#include <concepts>
#include <string>

template <typename T>
concept Loadable = requires(T a) {
    { a.loadFromFile(std::string{}) } -> std::convertible_to<bool>;
};

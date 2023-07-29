#pragma once

#include <functional>

#include "GameField.hpp"

namespace position {
using Func = std::function<sf::Vector2f(const GameField&)>;
}  // namespace position

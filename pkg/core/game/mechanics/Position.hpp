#pragma once

#include <functional>

#include "GameField.hpp"

namespace position {
using Func = std::function<float(const GameField&)>;

inline Func set(float x) {
    return [x](const GameField&) {
        return x;
    };
}

inline Func percent_width(int percent = 50) {
    return [percent](const GameField& field) {
        return field.left() + percent / 100 * field.width();
    };
}

inline Func percent_height(int percent = 50) {
    return [percent](const GameField& field) {
        return field.bottom() - percent / 100 * field.height();
    };
}

// add relative pos to other GameObjects
}  // namespace position

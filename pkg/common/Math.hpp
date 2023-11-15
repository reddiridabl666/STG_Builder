#pragma once

#include <numbers>

namespace math {
inline float to_degrees(float radians) {
    return radians * 180 / std::numbers::pi;
}

inline float to_radians(float degrees) {
    return degrees / 180 * std::numbers::pi;
}
}  // namespace math

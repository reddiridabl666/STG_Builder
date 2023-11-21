#pragma once

#include "Transformable.hpp"
#include "Utils.hpp"

template <typename T, typename... Args>
void normalise_width(T&& a, Args&&... args) {
    T max = utils::max(a, args...);
    a.set_width(max);
    (args.set_width(max), ...);
}

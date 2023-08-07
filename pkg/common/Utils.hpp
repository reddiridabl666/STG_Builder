#pragma once

#include <ostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    out << "[ ";
    for (const auto& el : vec) {
        out << el << ' ';
    }
    return out << ']';
}

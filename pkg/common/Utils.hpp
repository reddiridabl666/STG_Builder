#pragma once

#include <algorithm>
#include <ctime>
#include <deque>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {
inline std::string time_to_str(time_t time) {
    static constexpr size_t kBufferSize = 80;

    char buffer[kBufferSize];
    std::strftime(buffer, kBufferSize, "%Y-%m-%d %H:%M:%S", localtime(&time));
    std::string result(buffer);
    return result;
}

inline std::string day_to_str(time_t time) {
    static constexpr size_t kBufferSize = 80;

    char buffer[kBufferSize];
    std::strftime(buffer, kBufferSize, "%Y-%m-%d", localtime(&time));
    std::string result(buffer);
    return result;
}

template <typename T>
std::string to_string(const T& obj) {
    std::stringstream out;
    out << obj;
    return out.str();
}

}  // namespace utils

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    out << "[ ";
    for (const auto& el : vec) {
        out << el << ' ';
    }
    return out << ']';
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& vec) {
    out << "[ ";
    for (const auto& el : vec) {
        out << el << ' ';
    }
    return out << ']';
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, U>& map) {
    out << "[ ";
    for (const auto& [key, value] : map) {
        out << key << ' ';
    }
    return out << ']';
}

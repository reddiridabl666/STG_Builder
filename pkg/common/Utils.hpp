#pragma once

#include <fmt/ostream.h>

#include <deque>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <vector>

template <typename T>
std::string to_string(const T& obj) {
    std::stringstream out;
    out << obj;
    return out.str();
}

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

template <typename T, typename U>
struct fmt::formatter<std::unordered_map<T, U>> : fmt::ostream_formatter {};

template <typename T>
struct fmt::formatter<std::vector<T>> : fmt::ostream_formatter {};

template <typename T>
struct fmt::formatter<std::deque<T>> : fmt::ostream_formatter {};

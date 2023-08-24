#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <variant>

#include "Json.hpp"

template <typename U = std::variant<float, sf::Vector2f>>
struct FuncInfo {
    std::string name;
    nl::json args;
};

template <typename U>
void from_json(const nl::json& j, FuncInfo<U>& func) {
    j.at("type").get_to(func.name);
    j.at("args").get_to(func.args);
}

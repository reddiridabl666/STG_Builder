#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <variant>

#include "Json.hpp"

struct FuncInfo {
    std::string name;
    nl::json args;
};

inline void from_json(const nl::json& j, FuncInfo& func) {
    j.at("type").get_to(func.name);
    j.at("args").get_to(func.args);
}

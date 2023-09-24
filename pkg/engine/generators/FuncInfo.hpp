#pragma once

#include <string>

#include "Json.hpp"

struct FuncInfo {
    std::string type;
    nl::json args;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FuncInfo, type, args)

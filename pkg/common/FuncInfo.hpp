#pragma once

#include <string>

#include "Json.hpp"

struct FuncInfo {
    std::string type = "";
    nl::json args = nl::json::object();
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FuncInfo, type, args)

#pragma once

#include <string>

#include "Json.hpp"

struct FuncInfo {
    std::string type = "";
    nl::json args = nl::json::object();
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FuncInfo, type, args)

struct TimedFuncInfo : public FuncInfo {
    TimedFuncInfo() = default;
    TimedFuncInfo(const std::string& type, const nl::json& args, float time)
        : FuncInfo{type, args}, time(time) {}

    float time = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TimedFuncInfo, type, args, time)

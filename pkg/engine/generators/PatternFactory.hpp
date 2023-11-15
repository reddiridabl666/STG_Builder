#pragma once

#include "Json.hpp"
#include "Pattern.hpp"

struct PatternFactory {
    static std::unique_ptr<Pattern> create(const nl::json&);
};

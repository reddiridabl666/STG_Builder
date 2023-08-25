#pragma once

#include "App.hpp"

class AppFactory {
  public:
    App generate(const nl::json& game, const nl::json& entities, const std::string& level_prefix) const;
};

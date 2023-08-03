#pragma once

#include "App.hpp"

class AppFactory {
  public:
    ErrorOr<App> generate(Window& window, const nl::json& game, const nl::json& entities,
                          const std::string& level_prefix) const;
};

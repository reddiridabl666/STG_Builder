#pragma once

#include "Game.hpp"

namespace engine {
class GameFactory {
  public:
    static Game generate(Window& window, const nl::json& game, const nl::json& entities,
                         const std::string& base_dir);
};
}  // namespace engine

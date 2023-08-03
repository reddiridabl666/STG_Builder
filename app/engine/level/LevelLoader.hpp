#pragma once

#include "Level.hpp"

class LevelLoader {
  public:
    std::string prefix = "level";
    ErrorOr<Level> load_level(size_t number) const;
};

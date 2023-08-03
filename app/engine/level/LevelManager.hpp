#pragma once

#include "LevelLoader.hpp"

class LevelManager {
  public:
    LevelManager(size_t level_num, LevelLoader&& loader)
        : level_num_(level_num), loader_(std::move(loader)) {}

    ErrorOr<std::shared_ptr<Level>> current();
    ErrorOr<std::shared_ptr<Level>> start_next();

  private:
    size_t cur_num_ = 0;
    size_t level_num_;
    std::shared_ptr<Level> cur_level_;
    LevelLoader loader_;
};

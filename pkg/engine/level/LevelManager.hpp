#pragma once

#include "LevelLoader.hpp"

namespace engine {
class LevelManager {
  public:
    LevelManager(size_t level_num, LevelLoader&& loader)
        : level_num_(level_num), loader_(std::move(loader)) {}

    ErrorOr<std::shared_ptr<Level>> start_next(Window& window, AssetManager<sf::Texture>& textures);

  private:
    size_t cur_num_ = 0;
    size_t level_num_;

    LevelLoader loader_;
};
}  // namespace engine
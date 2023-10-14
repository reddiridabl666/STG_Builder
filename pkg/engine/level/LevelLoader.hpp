#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "AssetManager.hpp"
#include "Level.hpp"
#include "Loadable.hpp"

namespace engine {

class LevelLoader {
  public:
    LevelLoader(const std::string& prefix, const sf::FloatRect& field_size, const std::string& asset_dir)
        : prefix_(prefix), field_size_(field_size), assets_(asset_dir) {}

    ErrorOr<Level> load_level(Window& window, size_t number) const;

  private:
    std::string prefix_;
    sf::FloatRect field_size_;

    assets::Loader assets_;

    ErrorOr<GameField> load_field(Window& window, const nl::json& json) const;
};
}  // namespace engine

#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Level.hpp"
#include "Loadable.hpp"

template <Loadable T>
class AssetManager;

class LevelLoader {
  public:
    std::string prefix = "level";
    ErrorOr<Level> load_level(Window& window, AssetManager<sf::Texture>& textures, size_t number) const;

  private:
    ErrorOr<GameField> load_field(Window& window, AssetManager<sf::Texture>& textures, nl::json json) const;
};

#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Level.hpp"
#include "Loadable.hpp"

template <Loadable T>
class AssetManager;

class LevelLoader {
  public:
    LevelLoader(const std::string& prefix, const sf::FloatRect& field_size)
        : prefix_(prefix), field_size_(field_size) {}

    ErrorOr<Level> load_level(Window& window, AssetManager<sf::Texture>& textures, size_t number) const;

  private:
    sf::FloatRect field_size_;
    std::string prefix_;

    ErrorOr<GameField> load_field(Window& window, AssetManager<sf::Texture>& textures, nl::json json) const;
};

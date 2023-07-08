#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Drawable.hpp"
#include "Movable.hpp"

class SpriteObject : public Displayable {
  public:
    SpriteObject(const sf::Texture& texture) : sprite_(texture) {}

    sf::Drawable& get_drawable() {
        return sprite_;
    }

    sf::Transformable& get_transformable() {
        return sprite_;
    }

    void set_texture(const sf::Texture& texture) {
        sprite_.setTexture(texture);
    }

  private:
    sf::Sprite sprite_;
};

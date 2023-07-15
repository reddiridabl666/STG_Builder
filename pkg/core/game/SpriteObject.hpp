#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Displayable.hpp"

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

    sf::Vector2f get_size() const override {
        return sprite_.getTexture()->getSize() * sprite_.getScale();
    }

  private:
    sf::Sprite sprite_;
};

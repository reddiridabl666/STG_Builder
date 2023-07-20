#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

#include "Displayable.hpp"

class SpriteObject : public Displayable {
  public:
    SpriteObject(std::shared_ptr<sf::Texture>&& texture)
        : texture_(std::move(texture)), sprite_(*texture_) {}

    sf::Drawable& get_drawable() {
        return sprite_;
    }

    sf::Transformable& get_transformable() {
        return sprite_;
    }

    void set_texture(std::shared_ptr<sf::Texture>&& texture) {
        texture_ = std::move(texture);
        sprite_.setTexture(*texture);
    }

    sf::Vector2f get_size() const override {
        return texture_->getSize() * sprite_.getScale();
    }

  private:
    std::shared_ptr<sf::Texture> texture_;
    sf::Sprite sprite_;
};

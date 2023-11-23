#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

#include "Displayable.hpp"

class SpriteObject : public Displayable {
  public:
    SpriteObject(std::shared_ptr<sf::Texture>&& texture) : texture_(std::move(texture)), sprite_(*texture_) {}

    void draw(Window& window) const override {
        window.draw(sprite_);
    }

    sf::Transformable& transformable() override {
        return sprite_;
    }

    void set_texture(std::shared_ptr<sf::Texture>&& texture) {
        texture_ = std::move(texture);
        sprite_.setTexture(*texture_);
    }

    void set_texture_rect(const sf::IntRect& rect) {
        sprite_.setTextureRect(rect);
    }

    sf::Vector2u texture_size() {
        return texture_->getSize();
    }

    sf::Vector2f get_size() const override {
        return texture_->getSize() * sprite_.getScale();
    }

    sf::FloatRect get_bounds() const override {
        return sprite_.getGlobalBounds();
    }

  private:
    std::shared_ptr<sf::Texture> texture_;
    sf::Sprite sprite_;
};

#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <concepts>

#include "Displayable.hpp"

template <typename T>
concept Shape = std::is_base_of<sf::Shape, T>::value;

template <Shape T>
class ShapeObject : public Displayable {
  public:
    ShapeObject(const T& shape) : shape_(shape) {}

    sf::Drawable& get_drawable() {
        return shape_;
    }

    sf::Transformable& get_transformable() {
        return shape_;
    }

    void set_texture(const sf::Texture& texture) {
        shape_.setTexture(texture);
    }

    sf::Vector2f get_size() const override {
        return sprite_.getTexture()->getSize() * sprite_.getScale();
    }

  protected:
    T shape_;
};

class RectObject : public ShapeObject<sf::RectangleShape> {
  public:
    RectObject(size_t x = 0, size_t y = 0) : ShapeObject(sf::RectangleShape(sf::Vector2f{x, y})) {}

    sf::Vector2f get_size() const override {
        return shape_.getSize();
    }
};

class CircleObject : public ShapeObject<sf::CircleShape> {
  public:
    CircleObject(size_t radius = 0) : ShapeObject(sf::CircleShape(radius)) {}

    sf::Vector2f get_size() const override {
        auto radius = shape_.getRadius();
        return sf::Vector2f{radius, radius};
    }
};

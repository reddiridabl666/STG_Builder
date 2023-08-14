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

    sf::Drawable& drawable() override {
        return shape_;
    }

    sf::Transformable& transformable() override {
        return shape_;
    }

    sf::Vector2f get_size() const override {
        return shape_.getTexture()->getSize() * shape_.getScale();
    }

    T& shape() {
        return shape_;
    }

    const T& shape() const {
        return shape_;
    }

    sf::FloatRect get_bounds() const override {
        return shape_.getGlobalBounds();
    }

  protected:
    T shape_;
};

class RectObject : public ShapeObject<sf::RectangleShape> {
  public:
    RectObject(float x = 0, float y = 0) : ShapeObject(sf::RectangleShape(sf::Vector2f{x, y})) {}

    sf::Vector2f get_size() const override {
        return shape_.getSize();
    }
};

class CircleObject : public ShapeObject<sf::CircleShape> {
  public:
    CircleObject(float radius = 0) : ShapeObject(sf::CircleShape(radius)) {}

    sf::Vector2f get_size() const override {
        auto radius = shape_.getRadius();
        return sf::Vector2f{radius, radius};
    }
};

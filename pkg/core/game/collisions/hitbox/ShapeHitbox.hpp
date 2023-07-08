#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Geometry.hpp"
#include "Hitbox.hpp"

class CircleShapeHitbox : public CircleHitbox {
  public:
    CircleShapeHitbox(const sf::CircleShape& shape) : shape_(shape) {}

    FloatBox get_box() const override {
        return shape_.getGlobalBounds();
    }

    float radius() const override {
        return shape_.getRadius();
    }

    const sf::Vector2f& center() const override {
        return shape_.getTransform().transformPoint(shape_.getGeometricCenter());
    }

  private:
    const sf::CircleShape& shape_;
};

class RectShapeHitbox : public RectHitbox {
  public:
    RectShapeHitbox(const sf::RectangleShape& shape) : shape_(shape) {}

    FloatBox get_box() const override {
        return shape_.getGlobalBounds();
    }

    const FloatRect& rect() const override {
        sf::Transform matrix = shape_.getTransform();
        FloatRect::point_container points;

        for (size_t i = 0; i < 4; ++i) {
            points[i] = matrix.transformPoint(shape_.getPoint(i));
        }

        return FloatRect(points);
    }

  private:
    const sf::RectangleShape& shape_;
};

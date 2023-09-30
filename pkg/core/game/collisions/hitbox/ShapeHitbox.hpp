#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Geometry.hpp"
#include "Hitbox.hpp"

class CircleShapeHitbox : public CircleHitbox {
  public:
    CircleShapeHitbox(const sf::CircleShape& shape) : shape_(shape) {
        set_origin();
    }

    CircleShapeHitbox(sf::CircleShape&& shape) : shape_(std::move(shape)) {
        set_origin();
    }

    FloatBox get_frame() const {
        return shape_.getGlobalBounds();
    }

    float radius() const override {
        return shape_.getRadius();
    }

    const sf::Vector2f& center() const override {
        return shape_.getPosition();
    }

  private:
    void set_origin() {
        shape_.setOrigin(sf::Vector2f{radius(), radius()} / 2.f);
    }

    sf::CircleShape shape_;
};

class RectShapeHitbox : public RectHitbox {
  public:
    RectShapeHitbox(const sf::RectangleShape& shape) : shape_(shape) {
        set_origin();
    }

    RectShapeHitbox(sf::RectangleShape&& shape) : shape_(std::move(shape)) {
        set_origin();
    }

    FloatBox get_bounds() const {
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
    void set_origin() {
        shape_.setOrigin(shape_.getSize() / 2.f);
    }

    sf::RectangleShape shape_;
};

#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Geometry.hpp"
#include "Hitbox.hpp"
#include "ShapeObject.hpp"

class CircleShapeHitbox : public CircleHitbox, public CircleObject {
  public:
    CircleShapeHitbox(float radius, sf::Color color = sf::Color::White, size_t outline = 1) : CircleObject(radius) {
        set_origin();
        shape_.setFillColor(sf::Color::Transparent);
        shape_.setOutlineColor(color);
        shape_.setOutlineThickness(outline);
    }

    float radius() const override {
        return shape_.getRadius();
    }

    const sf::Vector2f& center() const override {
        return shape_.getPosition();
    }

  private:
    void set_origin() {
        shape_.setOrigin(sf::Vector2f{radius(), radius()});
    }
};

class RectShapeHitbox : public RectHitbox, public RectObject {
  public:
    RectShapeHitbox(const sf::Vector2f& size, sf::Color color = sf::Color::White, size_t outline = 1)
        : RectObject(size) {
        set_origin();
        shape_.setFillColor(sf::Color::Transparent);
        shape_.setOutlineColor(color);
        shape_.setOutlineThickness(outline);
    }

    FloatRect rect() const override {
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
};

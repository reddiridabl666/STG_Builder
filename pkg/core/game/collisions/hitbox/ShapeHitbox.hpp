#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Geometry.hpp"
#include "Hitbox.hpp"
#include "ShapeObject.hpp"

class CircleShapeHitbox : public CircleHitbox, public CircleObject {
  public:
    CircleShapeHitbox(float radius, sf::Color outline_color = sf::Color::White,
                      sf::Color fill_color = sf::Color::Transparent, size_t outline = 1)
        : CircleObject(radius, fill_color, outline_color, outline) {
        set_origin();
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
    RectShapeHitbox(const sf::Vector2f& size, sf::Color outline_color = sf::Color::White,
                    sf::Color fill_color = sf::Color::Transparent, size_t outline = 1)
        : RectObject(size, fill_color, outline_color, outline) {
        set_origin();
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

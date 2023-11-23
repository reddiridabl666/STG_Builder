#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <concepts>

#include "Displayable.hpp"

template <typename T>
concept Shape = std::is_base_of<sf::Shape, T>::value;

template <Shape T>
class ShapeObject : virtual public Displayable {
  public:
    ShapeObject(const T& shape) : shape_(shape) {}

    void draw(Window& window) const override {
        window.draw(shape_);
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
    RectObject(const sf::Vector2f& size = {}, sf::Color fill_color = sf::Color::White,
               sf::Color outline_color = sf::Color::Transparent, size_t outline = 0)
        : ShapeObject(make_shape(size, fill_color, outline_color, outline)) {}

    sf::Vector2f get_size() const override {
        return shape_.getSize();
    }

  private:
    sf::RectangleShape make_shape(const sf::Vector2f& size, sf::Color fill_color, sf::Color outline_color,
                                  size_t outline) {
        sf::RectangleShape shape(size);
        shape.setFillColor(fill_color);
        shape.setOutlineColor(outline_color);
        shape.setOutlineThickness(outline);
        return shape;
    }

    void set_size(const sf::Vector2f& size, bool) override {
        shape_.setSize(size);
    }
};

class CircleObject : public ShapeObject<sf::CircleShape> {
  public:
    CircleObject(float radius = 0, sf::Color fill_color = sf::Color::White,
                 sf::Color outline_color = sf::Color::Transparent, size_t outline = 0)
        : ShapeObject(make_shape(radius, fill_color, outline_color, outline)) {}

    sf::Vector2f get_size() const override {
        auto radius = shape_.getRadius();
        return sf::Vector2f{radius, radius};
    }

  private:
    sf::CircleShape make_shape(float radius, sf::Color fill_color, sf::Color outline_color, size_t outline) {
        sf::CircleShape shape(radius);
        shape.setFillColor(fill_color);
        shape.setOutlineColor(outline_color);
        shape.setOutlineThickness(outline);
        return shape;
    }

    void set_size(const sf::Vector2f& size, bool) override {
        shape_.setRadius(size.x / 2);
    }
};

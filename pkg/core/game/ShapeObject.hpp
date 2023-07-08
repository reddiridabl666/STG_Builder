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
    sf::Drawable& get_drawable() {
        return shape_;
    }

    sf::Transformable& get_transformable() {
        return shape_;
    }

    void set_texture(const sf::Texture& texture) {
        shape_.setTexture(texture);
    }

  private:
    T shape_;
};

using RectObject = ShapeObject<sf::RectangleShape>;

using CircleObject = ShapeObject<sf::CircleShape>;

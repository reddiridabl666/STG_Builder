#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class Window;

class Drawable {
  public:
    virtual sf::Drawable& get_drawable() = 0;
    virtual sf::Transformable& get_transformable() = 0;
    virtual ~Drawable() = default;
};

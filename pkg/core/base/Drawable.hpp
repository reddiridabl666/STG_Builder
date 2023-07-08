#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class Window;

class Drawable {
  public:
    virtual sf::Drawable& get_drawable() = 0;

    const sf::Drawable& get_drawable() const {
        return const_cast<Drawable*>(this)->get_drawable();
    }

    virtual ~Drawable() = default;
};

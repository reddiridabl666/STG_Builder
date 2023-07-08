#pragma once

#include <SFML/Graphics/Drawable.hpp>

class Window;

class Drawable {
  public:
    virtual sf::Drawable& drawable() = 0;

    const sf::Drawable& get_drawable() const {
        return const_cast<Drawable*>(this)->drawable();
    }

    virtual ~Drawable() = default;
};

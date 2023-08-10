#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include "Window.hpp"

class Drawable {
  public:
    virtual sf::Drawable& drawable() = 0;

    const sf::Drawable& drawable() const {
        return const_cast<Drawable*>(this)->drawable();
    }

    virtual void draw(Window& window) const {
        window.draw(*this);
    }

    virtual ~Drawable() = default;
};

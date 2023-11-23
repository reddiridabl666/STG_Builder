#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include "Window.hpp"

class Drawable {
  public:
    virtual void draw(Window& window) const = 0;
    virtual ~Drawable() = default;
};

#pragma once

#include "Controls.hpp"
#include "Window.hpp"

namespace ui {
class KeyMapInput {
  public:
    // KeyMapInput(Window&);

    static void draw(KeyControls& keys, Window& window);

    //     ~KeyMapInput();

    //   private:
    //     static inline sf::Keyboard::Key pressed_ = sf::Keyboard::Unknown;
    //     Window& window_;
};
}  // namespace ui

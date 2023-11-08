#pragma once

#include "Controls.hpp"
#include "Window.hpp"

namespace ui {
class KeyMapInput {
  public:
    static void draw(KeyControls& keys, Window& window);
};
}  // namespace ui

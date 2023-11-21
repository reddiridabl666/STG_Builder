#pragma once

#include "Button.hpp"

namespace engine {
class PauseMenu {
  public:
    PauseMenu(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& continue_btn, Button&& quit,
              float offset);

    void draw(Window& window);

    void set_pos(const sf::Vector2f& pos);

  private:
    std::unique_ptr<Displayable> bg_;
    Text msg_;
    Button continue_;
    Button quit_;

    float offset_;
};
}  // namespace engine

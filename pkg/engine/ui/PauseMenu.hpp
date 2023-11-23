#pragma once

#include "Button.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class PauseMenu {
  public:
    PauseMenu(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
              std::unique_ptr<Button>&& continue_btn, std::unique_ptr<Button>&& quit, float offset);

    void draw(Window& window);

    void set_pos(const sf::Vector2f& pos);

  private:
    VerticalLayout layout_;
};
}  // namespace engine

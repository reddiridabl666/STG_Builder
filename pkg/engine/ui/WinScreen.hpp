#pragma once

#pragma once

#include "Button.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class WinScreen {
  public:
    WinScreen(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
              std::unique_ptr<Button>&& save_score, std::unique_ptr<Button>&& quit, float offset_x, float offset_y);

    void set_pos(const sf::Vector2f& pos);

    sf::Vector2f get_size() {
        return layout_.get_size();
    }

    void draw(Window& window) const;

  private:
    VerticalLayout layout_;
};
}  // namespace engine

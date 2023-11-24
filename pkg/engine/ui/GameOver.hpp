#pragma once

#include "Button.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class GameOver {
  public:
    GameOver(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
             std::unique_ptr<Button>&& retry, std::unique_ptr<Button>&& quit, float offset, float margin);

    void set_pos(const sf::Vector2f& pos);

    sf::Vector2f get_size() {
        return layout_.get_size();
    }

    void draw(Window& window) const;

  private:
    VerticalLayout layout_;
};
}  // namespace engine

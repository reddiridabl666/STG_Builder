#pragma once

#include "Button.hpp"
#include "GameBus.hpp"

namespace engine {
class GameOver {
  public:
    GameOver(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& retry, Button&& quit);

    void set_pos(const sf::Vector2f& pos);

    sf::Vector2f get_size() {
        return bg_->get_size();
    }

    void draw(Window& window) const {
        bg_->draw(window);
        msg_.draw(window);
        retry_.draw(window);
        quit_.draw(window);
    }

  private:
    std::unique_ptr<Displayable> bg_;
    Text msg_;
    Button retry_;
    Button quit_;

    float margin_ = 200;
    float offset_ = 50;
};
}  // namespace engine

#pragma once

#include "Button.hpp"
#include "VerticalLayout.hpp"

class MainMenu {
  public:
    MainMenu(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
             std::unique_ptr<Button>&& start, std::unique_ptr<Button>&& settings, std::unique_ptr<Button>&& quit,
             float offset);

    void draw(Window& window);

    void set_pos(const sf::Vector2f& pos);

  private:
    // std::unique_ptr<Displayable> bg_;
    // Text msg_;
    // Button start_;
    // Button settings_;
    // Button quit_;

    VerticalLayout layout_;
};

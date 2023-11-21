#pragma once

#include "Button.hpp"

class MainMenu {
  public:
    MainMenu(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& start, Button&& settings,
             Button&& quit, float offset);

    void draw(Window& window);

    void set_pos(const sf::Vector2f& pos);

  private:
    std::unique_ptr<Displayable> bg_;
    Text msg_;
    Button start_;
    Button settings_;
    Button quit_;

    float offset_;
};

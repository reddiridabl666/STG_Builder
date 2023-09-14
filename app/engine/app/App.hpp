#pragma once

#include "Game.hpp"
#include "Window.hpp"

struct WindowInfo {
    std::string name;
    sf::Vector2i size;

    Window make_window() const {
        return Window(name, size.x, size.y);
    }
};

class App {
  public:
    App(const WindowInfo& window_info, Game&& game);

    void run();

  private:
    void draw_ui();

    Window window_;
    Game game_;
};

#pragma once

#include "Game.hpp"
#include "Window.hpp"

namespace engine {
class App {
  public:
    App(Window& window, Game&& game);

    void run();

  private:
    void draw_ui();

    Window& window_;
    Game game_;
};
}  // namespace engine

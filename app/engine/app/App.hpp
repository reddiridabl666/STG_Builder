#pragma once

#include "Game.hpp"
#include "Window.hpp"

namespace engine {
class App {
  public:
    App(Window& window, Game<>&& game);

    void run();

    ~App();

  private:
    void draw_ui();

    Window& window_;
    Game<> game_;
    bool paused_ = false;
};
}  // namespace engine

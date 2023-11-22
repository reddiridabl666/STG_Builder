#pragma once

#include "Game.hpp"
#include "Window.hpp"

namespace engine {
class App {
  public:
    App(Window& window, const std::filesystem::path& game_path);

    void run();

    ~App();

  private:
    void draw_ui();
    std::unique_ptr<Game<>> load_game();

    Window& window_;
    std::filesystem::path game_path_;
    std::unique_ptr<Game<>> game_;
};
}  // namespace engine

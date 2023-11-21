#pragma once

#include "Game.hpp"
#include "Window.hpp"
#include "ui/MainMenu.hpp"

namespace engine {
class App {
  public:
    App(Window& window, MainMenu&& menu, const std::filesystem::path& game_path);

    void run();

    ~App();

  private:
    enum class State {
        MainMenu,
        Settings,
        Game,
    };

    void draw_ui();
    std::unique_ptr<Game<>> load_game();

    Window& window_;
    std::filesystem::path game_path_;
    std::unique_ptr<Game<>> game_;
    MainMenu main_menu_;
    State state_ = State::MainMenu;
};
}  // namespace engine

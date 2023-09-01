#pragma once

#include <functional>
#include <vector>

#include "Window.hpp"
#include "ui/elements/GameInfo.hpp"

namespace builder {
class App {
  public:
    App(const std::string& games_dir, const std::string& name, uint width, uint height);

    enum class State {
        MainMenu,
        GameMenu,
        LevelEditor
    };

    void run();

  private:
    void main_menu();

    void state_action();

    Window window_;
    State state_;
    ui::GameInfoBox games_;
    std::string games_dir_;
};
}  // namespace builder

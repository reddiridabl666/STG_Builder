#pragma once

#include <filesystem>
#include <functional>
#include <vector>

#include "AssetManager.hpp"
#include "Window.hpp"
#include "ui/elements/Box.hpp"

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

    std::vector<std::unique_ptr<ui::Element>> load_games(const std::filesystem::path& games_dir);

    Window window_;
    State state_;
    std::string games_dir_;

    AssetManager<sf::Texture> textures_;
    ui::Box games_;
};
}  // namespace builder

#pragma once

#include <filesystem>
#include <functional>
#include <stack>
#include <vector>

#include "AssetManager.hpp"
#include "EditableGame.hpp"
#include "GameBuilder.hpp"
#include "Observable.hpp"
#include "StateManager.hpp"
#include "Window.hpp"
#include "ui/elements/Box.hpp"

namespace builder {

namespace fs = std::filesystem;

class App {
  public:
    App(const std::string& games_dir, const std::string& name, uint width, uint height);

    enum class State {
        Undefined,
        MainMenu,
        GameMenu,
        LevelEditor,
        GamePreview,
        Back,
    };

    void run() noexcept;

    ~App();

  private:
    void draw_ui();

    void on_state_start(State state);
    void on_state_end(State state);

    std::unique_ptr<ui::Element> back_button();

    ui::DefaultBox::Items load_games();
    ui::DefaultBox::Items load_levels();
    std::unique_ptr<ui::Element> make_menu();

    std::function<void()> game_choice(const fs::path& current_game);
    std::function<void()> new_game();

    Window window_;
    StateManager<State> state_;

    fs::path games_dir_;
    fs::path current_game_;
    size_t games_num_;

    GameBuilder builder_;
    std::unique_ptr<EditableGame> game_ = nullptr;

    assets::Textures textures_;
    std::unordered_map<std::string, std::unique_ptr<ui::Element>> ui_;

    sf::Clock game_clock_;
};
}  // namespace builder

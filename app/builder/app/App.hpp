#pragma once

#include <filesystem>
#include <functional>
#include <stack>
#include <vector>

#include "AssetManager.hpp"
#include "Observable.hpp"
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
        Back,
    };

    void run() noexcept;

  private:
    static constexpr const char* kFallbackImage = "fallback.png";

    void main_loop(const std::function<void()>& cb);

    void draw_ui();

    void schedule_state_change(State state);
    void resolve_state_change();

    void set_state(State state);
    void set_prev_state();
    State state() const;

    void on_state_start(State state);
    void on_state_end(State state);

    std::unique_ptr<ui::Element> back_button();

    ui::Box::Items load_games();
    ui::Box::Items load_levels();

    std::function<void()> game_choice(const fs::path& current_game);
    std::function<void()> new_game();

    Window window_;
    std::stack<State> states_;
    State next_state_ = State::Undefined;

    fs::path games_dir_;
    fs::path current_game_;
    size_t games_num_;

    AssetManager<sf::Texture> textures_;
    std::unordered_map<std::string, std::unique_ptr<ui::Element>> ui_;
};
}  // namespace builder

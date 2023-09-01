#include "App.hpp"

#include <SFML/System/Clock.hpp>

#include "Json.hpp"
#include "Messages.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/GameInfo.hpp"
#include "ui/elements/LangChanger.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif
namespace builder {

namespace fs = std::filesystem;

App::App(const std::string& games_dir, const std::string& name, uint width, uint height)
    : window_(name, width, height),
      state_(State::MainMenu),
      games_dir_(games_dir),
      textures_(games_dir),
      games_(load_games(games_dir_), ImVec2{400, 400}, window_.get_center()) {}

std::vector<std::unique_ptr<ui::Element>> App::load_games(const fs::path& games_dir) {
    Lang::set(Lang::EN);

    std::vector<std::unique_ptr<ui::Element>> res;

    res.push_back(std::make_unique<ui::ImageButton>(
        std::bind(message, Message::CreateGame), *textures_.get("plus_invert.png"), ImVec2{50, 50},
        [] {
            fmt::println("Button pressed");
        },
        ImVec2{0, 70}));

    if (!fs::is_directory(games_dir)) {
        return res;
    }

    res.reserve(4);

    for (const auto& dir : fs::directory_iterator(games_dir)) {
        if (!dir.is_directory()) {
            continue;
        }

        auto game = json::read(dir.path() / "game.json");
        if (!game) {
            continue;
        }
#ifdef DEBUG
        LOG(fmt::format("Found game: {}", game->at("name").template get<std::string>()));
#endif
        res.push_back(std::make_unique<ui::GameInfo>(game->template get<ui::GameInfo>()));
    }

#ifdef DEBUG
    for (size_t i = 0; i < 2; ++i) {
        res.push_back(std::make_unique<ui::GameInfo>("Another game", "Just for UI testing"));
    }
#endif

    return res;
}

void App::run() {
    sf::Clock timer;

    while (window_.is_open()) {
        window_.process_events();
        window_.clear();

        window_.update_ui();

        // ImGui::ShowDemoWindow();

        state_action();

        window_.display();
    }
}

void App::main_menu() {
    games_.draw(window_);
}

void App::state_action() {
    ui::LangChanger::draw(ImVec2{1200, 660}, ImVec2{30, 30});

    switch (state_) {
        case State::MainMenu:
            return main_menu();
        case State::GameMenu:
            return;
        case State::LevelEditor:
            return;
    }
}
}  // namespace builder

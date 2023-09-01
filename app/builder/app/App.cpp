#include "App.hpp"

#include <SFML/System/Clock.hpp>
#include <filesystem>

#include "Json.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace {
namespace fs = std::filesystem;

std::vector<ui::GameInfo> load_games(const fs::path& games_dir) {
    std::vector<ui::GameInfo> res;

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
        res.push_back(game->template get<ui::GameInfo>());
    }

#ifdef DEBUG
    for (size_t i = 0; i < 2; ++i) {
        res.push_back(ui::GameInfo{
            "Another game",
            "Just for UI testing",
        });
    }
#endif

    return res;
}
}  // namespace

namespace builder {
App::App(const std::string& games_dir, const std::string& name, uint width, uint height)
    : window_(name, width, height), state_(State::MainMenu), games_dir_(games_dir) {}

void App::run() {
    auto center = ImVec2{window_.get_size().x / 2.f, window_.get_size().y / 2.f};
    games_ = ui::GameInfoBox(load_games(games_dir_), ImVec2{400, 400}, center);
    Lang::set(Lang::EN);

    sf::Clock timer;

    while (window_.is_open()) {
        window_.process_events();
        window_.clear();

        window_.update_ui();

        ImGui::ShowDemoWindow();

        state_action();

        window_.display();
    }
}

void App::main_menu() {
    games_.draw(window_);
}

void App::state_action() {
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

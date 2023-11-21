#include "App.hpp"

#include <SFML/System/Clock.hpp>

#include "GameFactory.hpp"
#include "GameState.hpp"
#include "Utils.hpp"
#include "ui/elements/StatBox.hpp"

namespace engine {
App::App(Window& window, const std::filesystem::path& game_path)
    : window_(window), game_path_(game_path), game_(load_game()) {}

std::unique_ptr<Game<>> App::load_game() {
    auto game_json = json::read(game_path_ / "game.json");
    if (!game_json) {
        throw game_json.error();
    }

    auto entities_json = json::read(game_path_ / "entities.json");
    if (!entities_json) {
        throw entities_json.error();
    }

    return GameFactory::generate_unique<>(window_, *game_json, *entities_json, game_path_);
}

void App::run() {
    sf::Clock timer;
    game_->register_events();

    window_.add_handler("app_zoom", sf::Event::KeyReleased, [this](const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Equal) {
            game_->zoom(0.8);
        }
        if (event.key.code == sf::Keyboard::Dash) {
            game_->zoom(1.25);
        }
    });

    window_.main_loop([this, &timer] {
        auto status = game_->render(timer.restart().asSeconds());
        if (status == Game<>::Status::Restart) {
            game_ = load_game();
            game_->register_events();
        }

        if (status == Game<>::Status::Ended) {
            window_.close();
        }
    });
}

App::~App() {
    window_.remove_handler("app_engine_pause");
}
}  // namespace engine

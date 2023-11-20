#include "App.hpp"

#include <SFML/System/Clock.hpp>

#include "GameState.hpp"
#include "Utils.hpp"
#include "ui/elements/StatBox.hpp"

namespace engine {
App::App(Window& window, Game<>&& game) : window_(window), game_(std::move(game)) {
    window_.add_handler("app_engine_pause", sf::Event::KeyReleased, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            paused_ = !paused_;
        }
    });
}

void App::run() {
    sf::Clock timer;
    game_.register_events();

    window_.add_handler("app_zoom", sf::Event::KeyReleased, [this](const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Equal) {
            game_.zoom(0.8);
        }
        if (event.key.code == sf::Keyboard::Dash) {
            game_.zoom(1.25);
        }
    });

    window_.main_loop([this, &timer] {
        if (!paused_) {
            game_.render(timer.restart().asSeconds());
        } else {
            timer.restart();
            game_.draw_objects();
        }
    });
}

App::~App() {
    window_.remove_handler("app_engine_pause");
}
}  // namespace engine

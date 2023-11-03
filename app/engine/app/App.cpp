#include "App.hpp"

#include <SFML/System/Clock.hpp>

#include "GameInfo.hpp"
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

    while (window_.is_open()) {
        window_.process_events();
        window_.clear();

        window_.update_ui();
        if (!paused_) {
            game_.render(timer.restart().asSeconds());
        } else {
            timer.restart();
            game_.draw_objects();
        }

        window_.display();
    }
}

App::~App() {
    window_.remove_handler("app_engine_pause");
}
}  // namespace engine

#include "App.hpp"

#include <SFML/System/Clock.hpp>

#include "GameInfo.hpp"
#include "Utils.hpp"
#include "ui/elements/StatBox.hpp"

namespace engine {
App::App(Window& window, Game<>&& game) : window_(window), game_(std::move(game)) {}

void App::run() {
    sf::Clock timer;

    while (window_.is_open()) {
        window_.process_events();
        window_.clear();

        window_.update_ui();

        auto err = game_.render(timer.restart().asSeconds());
        if (err) {
            throw std::runtime_error(err.message());
        }

        window_.display();
    }
}
}  // namespace engine

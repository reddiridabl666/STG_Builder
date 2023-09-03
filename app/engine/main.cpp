#include <iostream>

#include "AppFactory.hpp"
#include "Window.hpp"

static const std::string kBase = "../../../test_games/test_game/";

int main() {
    auto game_json = json::read(kBase + "game.json");
    if (!game_json) {
        std::cout << game_json.error() << std::endl;
        return -1;
    }

    auto entities_json = json::read(kBase + "entities.json");
    if (!entities_json) {
        std::cout << entities_json.error() << std::endl;
        return -1;
    }

    try {
        auto app = AppFactory{}.generate(*game_json, *entities_json, kBase);
        app.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

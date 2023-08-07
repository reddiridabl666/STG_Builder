#include <iostream>

#include "AppFactory.hpp"
#include "Window.hpp"

static const std::string kBase = "/home/leonid/Projects/stg_builder/test_game_data/";

int main() {
    Window window("STG_Builder", 1280, 720);

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

    auto app = AppFactory{}.generate(window, *game_json, *entities_json, kBase);
    if (!app) {
        std::cout << app.error() << std::endl;
        return -1;
    }

    auto err = app->run();
    if (err) {
        std::cout << err << std::endl;
        return -1;
    }
    return 0;
}

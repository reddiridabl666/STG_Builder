#include <filesystem>
#include <iostream>

#include "App.hpp"
#include "GameFactory.hpp"
#include "Window.hpp"
#include "WindowInfo.hpp"

static const std::filesystem::path kBase = "../../../test_games/test_game";

int main() {
    auto game_json = json::read(kBase / "game.json");
    if (!game_json) {
        std::cout << game_json.error() << std::endl;
        return -1;
    }

    try {
        auto window = game_json->get<WindowInfo>().make_window();
        auto app = engine::App{window, kBase};
        app.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

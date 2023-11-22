#include <filesystem>
#include <iostream>

#include "App.hpp"
#include "GameFactory.hpp"
#include "Window.hpp"
#include "WindowInfo.hpp"

int main(int argc, char** argv) {
    std::filesystem::path base_path;

    if (argc < 2) {
        base_path = std::filesystem::current_path();
    } else {
        base_path = argv[1];
    }

    auto game_json = json::read(base_path / "game.json");
    if (!game_json) {
        std::cout << game_json.error() << std::endl;
        return -1;
    }

    auto window = game_json->get<WindowInfo>().make_window();
    auto app = engine::App{window, base_path};
    app.run();

    return 0;
}

#include <iostream>

#include "App.hpp"
#include "AssetManager.hpp"
#include "WindowSFML.hpp"

int main() {
    // WindowSFML window("STG_Builder", 640, 480);
    // App app(window);
    // return app.run();

    AssetLoader loader("/home/leonid/Projects/stg_builder/test_game_dat");
    AssetStorage<sf::Texture> storage;
    AssetManager<sf::Texture> manager(std::move(loader), std::move(storage));

    auto res = manager.get("image.jpg");
    if (res) {
        std::cout << res.value()->getSize().x << ',' << res.value()->getSize().y << std::endl;
    } else {
        std::cout << res.error().message() << std::endl;
    }

    return 0;
}

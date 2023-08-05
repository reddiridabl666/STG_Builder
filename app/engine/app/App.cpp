#include "App.hpp"

#include "test.hpp"

App::App(Window& window, AssetManager<sf::Texture>&& textures, AssetManager<sf::SoundBuffer>&& sounds,
         ObjectTypeFactory::res_type&& types, LevelManager&& levels)
    : window_(window),
      textures_(std::move(textures)),
      sounds_(std::move(sounds)),
      types_(std::move(types)),
      levels_(std::move(levels)) {}

bool App::run() {
    // ui_elements_.push_back(std::make_shared<Test>());

    while (window_.is_open()) {
        draw_objects();
        // window_.draw_ui(ui_elements_);

        window_.process_events();
        window_.display();
    }

    return 0;
}

void App::draw_objects() {
    for (const auto& obj : objects_) {
        window_.draw(obj);
    }
}

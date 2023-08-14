#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <unordered_map>
#include <vector>

#include "AssetManager.hpp"
#include "GameField.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Window.hpp"

class App {
  public:
    App(Window& window, AssetManager<sf::Texture>&& textures, AssetManager<sf::SoundBuffer>&& sounds,
        ObjectTypeFactory::res_type&& types, LevelManager&& levels);

    ErrorPtr run();

  private:
    void draw_objects();

    ErrorPtr update(float delta_time);
    ErrorPtr update_level();

    ErrorOr<GameObject> generate_object(const ObjectOptions& opts);

    ErrorPtr generate_objects();

    void clear_dead();
    void update_object_status();

    void draw_ui();

    Window& window_;

    AssetManager<sf::Texture> textures_;
    AssetManager<sf::SoundBuffer> sounds_;

    std::unordered_map<std::string, GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    std::shared_ptr<Level> level_;
    LevelManager levels_;

    // Collection<UiElement> ui_elements_;
};

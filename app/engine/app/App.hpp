#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
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

    Window& window_;

    AssetManager<sf::Texture> textures_;
    AssetManager<sf::SoundBuffer> sounds_;

    std::vector<GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    std::shared_ptr<Level> level_;
    LevelManager levels_;
};

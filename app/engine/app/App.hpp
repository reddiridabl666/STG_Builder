#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssetManager.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Window.hpp"

class App {
  public:
    App(Window& window, AssetManager<sf::Texture>&& textures, AssetManager<sf::SoundBuffer>&& sounds,
        ObjectTypeFactory::res_type&& types, LevelManager&& levels);

    bool run();

  private:
    void draw_objects();

    // void generate_objects();

    Window& window_;

    AssetManager<sf::Texture> textures_;
    AssetManager<sf::SoundBuffer> sounds_;

    std::vector<GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    LevelManager levels_;
};

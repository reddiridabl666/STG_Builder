#pragma once

#include "AssetManager.hpp"
#include "GameField.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"
#include "Window.hpp"

class Game {
  public:
    Game(const Window& window, PlayerList&& players, AssetManager<sf::Texture>& textures,
         AssetManager<sf::SoundBuffer>& sounds, ObjectTypeFactory::res_type&& types, LevelManager&& levels);

  private:
    void draw_objects();

    Error update(float delta_time);
    Error update_level();

    ErrorOr<GameObject> generate_object(const ObjectOptions& opts);

    Error generate_objects();

    void clear_dead();
    Error generate_players();

    void draw_ui();

    const Window& window_;

    AssetManager<sf::Texture>& textures_;
    AssetManager<sf::SoundBuffer>& sounds_;

    std::unordered_map<std::string, GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    PlayerList player_types_;

    std::shared_ptr<Level> level_;
};

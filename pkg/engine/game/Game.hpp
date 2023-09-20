#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "AssetManager.hpp"
#include "GameField.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"
#include "RTree.hpp"
#include "Window.hpp"

namespace engine {
class Game {
  public:
    Game(Window& window, PlayerList&& players, AssetManager<sf::Texture>&& textures,
         AssetManager<sf::SoundBuffer>&& sounds, ObjectTypeFactory::res_type&& types, LevelManager&& levels,
         int fps);

    Error render(float delta_time);

    auto& textures() {
        return textures_;
    }

    void scroll(float);
    void zoom(float);

    Error choose_level(size_t);

    void render_debug();
    void reload_objects();

    GameObject* object_by_pos(const sf::Vector2f& pos);

    int fps() const {
        return fps_;
    }

  private:
    Error update(float delta_time);

    void draw_objects();

    Error update_level();

    ErrorOr<GameObject> generate_object(const ObjectOptions& opts);

    Error generate_objects();

    void clear_dead();
    Error generate_players();

    void draw_ui();

    Window& window_;

    AssetManager<sf::Texture> textures_;
    AssetManager<sf::SoundBuffer> sounds_;

    std::unordered_map<std::string, GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    PlayerList player_types_;

    std::shared_ptr<Level> level_;
    LevelManager levels_;

    int fps_;
    RTree<> rtree_;
};
}  // namespace engine

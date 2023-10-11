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
template <typename RTreeType = RTree<>>
class Game {
  public:
    Game(Window& window, PlayerList&& players, AssetManager<sf::Texture>&& textures,
         AssetManager<sf::SoundBuffer>&& sounds, ObjectTypeFactory::res_type&& types, LevelManager&& levels,
         int fps);

    Error render(float delta_time);

    int fps() const {
        return fps_;
    }

    void clear();

    void set_object_pos(GameObject& obj, const sf::Vector2f& pos);

  protected:
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
    RTreeType rtree_;
};
}  // namespace engine

#include "Game.tpp"

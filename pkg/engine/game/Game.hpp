#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "AssetManager.hpp"
#include "GameField.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"
#include "PlayerLoader.hpp"
#include "RTree.hpp"
#include "SideMenu.hpp"
#include "Window.hpp"

namespace engine {
template <typename RTreeType = RTree<>>
class Game {
  public:
    Game(Window& window, SpriteObject&& bg, SideMenu&& menu, PlayerLoader&& player_loader, assets::Manager&& assets,
         ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps);

    void render(float delta_time);

    void clear();
    void draw_objects();

  protected:
    void update(float delta_time);

    void update_level();

    std::shared_ptr<GameObject> generate_object(const ObjectOptions& opts);

    void add_object(std::shared_ptr<GameObject>&&);
    void add_player(std::shared_ptr<GameObject>&&);

    void add_object_to_rtree(const std::shared_ptr<GameObject>&);
    void remove_object_from_rtree(const std::shared_ptr<GameObject>&);

    void generate_objects();
    void generate_players();

    void check_collisions();

    void clear_dead();

    void draw_ui();

    void draw_with_default_view(Drawable&);

    Window& window_;
    SpriteObject bg_;
    SideMenu menu_;

    assets::Manager assets_;

    std::unordered_map<std::string, std::shared_ptr<GameObject>> objects_;
    ObjectTypeFactory::res_type types_;

    std::shared_ptr<Level> level_;
    LevelManager levels_;

    int fps_;
    RTreeType rtree_;
    RTreeType hitboxes_;

    PlayerLoader player_loader_;
};
}  // namespace engine

#include "Game.tpp"

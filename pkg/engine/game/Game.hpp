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

    Error render(float delta_time);

    int fps() const {
        return fps_;
    }

    void clear();
    void draw_objects();

  protected:
    Error update(float delta_time);

    Error update_level();

    ErrorOr<GameObject> generate_object(const ObjectOptions& opts);

    void add_object(GameObject&&);
    Error generate_objects();
    Error generate_players();

    void clear_dead();

    void draw_ui();

    void draw_with_default_view(Drawable&);

    Window& window_;
    SpriteObject bg_;
    SideMenu menu_;

    assets::Manager assets_;

    std::unordered_map<std::string, GameObject> objects_;
    ObjectTypeFactory::res_type types_;

    std::shared_ptr<Level> level_;
    LevelManager levels_;

    int fps_;
    RTreeType rtree_;

    PlayerLoader player_loader_;
};
}  // namespace engine

#include "Game.tpp"

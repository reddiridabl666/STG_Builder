#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "AssetManager.hpp"
#include "GameField.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"
#include "PlayerManager.hpp"
#include "RTree.hpp"
#include "Window.hpp"
#include "ui/GameOver.hpp"
#include "ui/SideMenu.hpp"

namespace engine {
template <typename RTreeType = RTree<>>
class Game {
  public:
    enum class Status {
        Running,
        Restart,
        Paused,
        GameOver,
        Ended,
    };

    Game(Window& window, SpriteObject&& bg, SideMenu&& menu, GameOver&& game_over, PlayerManager&& player_manager,
         assets::Manager&& assets, ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps);

    Game() = delete;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = default;

    Game(Game&&);

    void register_events();

    Status render(float delta_time);
    void zoom(float);

    void clear();
    void draw_objects();

    ~Game();

  protected:
    void update(float delta_time);

    void update_level();

    std::shared_ptr<GameObject> generate_object(const ObjectOptions& opts);

    void add_object(std::shared_ptr<GameObject>&&);
    void add_player(std::shared_ptr<GameObject>&&);
    void add_objects(std::vector<std::shared_ptr<GameObject>>&&);

    void add_object_to_rtree(const std::shared_ptr<GameObject>&);
    void remove_object_from_rtree(const std::shared_ptr<GameObject>&);

    void generate_objects();
    void generate_players();

    void check_collisions();
    void resolve_timed_actions(float delta_time);

    void clear_dead();

    void draw_ui();

    void draw_with_default_view(Drawable&);

    void fire_key_event(sf::Keyboard::Key, const std::string& suffix = "");

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

    PlayerManager player_manager_;

    GameOver game_over_;

  private:
    bool events_registered_ = false;
    Status status_ = Status::Running;
};
}  // namespace engine

#include "Game.tpp"

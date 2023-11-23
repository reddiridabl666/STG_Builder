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
#include "ui/MainMenu.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/SideMenu.hpp"

namespace engine {
template <typename RTreeType = RTree<>>
class Game {
  public:
    enum class Status {
        MainMenu,
        Running,
        Restart,
        Paused,
        Settings,
        GameOver,
        Ended,
    };

    Game(Window& window, SpriteObject&& bg, SideMenu&& menu, MainMenu&& main_menu, GameOver&& game_over,
         PauseMenu&& pause_menu, PlayerManager&& player_manager, assets::Manager&& assets,
         ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps, float level_transition);

    Game() = delete;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = default;

    Game(Game&&);

    void register_events();

    void start();
    void to_main_menu();

    Status render(float delta_time);
    void zoom(float);

    void clear();
    void reset();
    void draw_objects();

    ~Game();

  protected:
    void update(float delta_time);

    bool update_level(float delta_time);

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

    template <typename T>
    void draw_with_default_view(T&&);

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

    MainMenu main_menu_;
    GameOver game_over_;
    PauseMenu pause_menu_;

    float level_transition_;
    float level_transition_cur_;

  private:
    bool events_registered_ = false;
    Status status_ = Status::MainMenu;
};
}  // namespace engine

#include "Game.tpp"

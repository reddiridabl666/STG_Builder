#include "ActionQueue.hpp"
#include "AssetManager.hpp"
#include "GameBus.hpp"
#include "GameState.hpp"
#include "LevelManager.hpp"
#include "ObjectTypeFactory.hpp"
#include "PlayerManager.hpp"
#include "Utils.hpp"
#include "ui/GameOver.hpp"
#include "ui/MainMenu.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/SideMenu.hpp"

#ifdef DEBUG
#include "ui/elements/StatBox.hpp"
#endif

namespace engine {
template <typename RTreeType>
Game<RTreeType>::Game(Window& window, SpriteObject&& bg, SideMenu&& menu, MainMenu&& main_menu, GameOver&& game_over,
                      PauseMenu&& pause_menu, PlayerManager&& player_manager, assets::Manager&& assets,
                      ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps)
    : window_(window),
      bg_(std::move(bg)),
      menu_(std::move(menu)),
      assets_(std::move(assets)),
      types_(std::move(types)),
      levels_(std::move(levels)),
      fps_(fps),
      player_manager_(std::move(player_manager)),
      main_menu_(std::move(main_menu)),
      game_over_(std::move(game_over)),
      pause_menu_(std::move(pause_menu)) {}

template <typename RTreeType>
void Game<RTreeType>::register_events() {
    window_.add_handler("game_key_pressed", sf::Event::KeyPressed, [this](const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape) {
            status_ = (status_ == Status::Paused ? Status::Running : Status::Paused);
            return;
        }
        fire_key_event(event.key.code, "_pressed");
    });

    window_.add_handler("game_key_released", sf::Event::KeyReleased, [this](const sf::Event& event) {
        fire_key_event(event.key.code, "_released");
    });

    GameBus::get().on(GameEvent::BulletShot, [this](const nl::json& payload) {
        std::string type_name = payload.value("type", "");

        auto obj = objects_.at(payload.value("object", ""));
        auto pattern = types_.at(type_name).pattern(payload.value("pattern", ""));
        if (pattern) {
            add_objects(pattern->create(obj, types_, assets_));
        }
    });

    GameBus::get().on(GameEvent::GameRestarted, [this](const auto&) {
        status_ = Status::Restart;
    });

    GameBus::get().on(GameEvent::GameEnded, [this](const auto&) {
        status_ = Status::Ended;
    });

    GameBus::get().on(GameEvent::GameUnpaused, [this](const auto&) {
        if (status_ == Status::Paused) {
            status_ = Status::Running;
        }
    });

    GameBus::get().on(GameEvent::GameStarted, [this](const auto&) {
        start();
    });

    GameBus::get().on(GameEvent::SettingsOpened, [this](const auto&) {
        status_ = Status::Settings;
    });

    GameBus::get().on(GameEvent::MainMenuOpened, [this](const auto&) {
        to_main_menu();
    });

    events_registered_ = true;
}

template <typename RTreeType>
Game<RTreeType>::Game(Game&& other)
    : Game(other.window_, std::move(other.bg_), std::move(other.menu_), std::move(other.player_manager_),
           std::move(other.assets_), std::move(other.types_), std::move(other.levels_), other.fps_) {}

template <typename RTreeType>
Game<RTreeType>::~Game() {
    if (events_registered_) {
        window_.remove_handler("game_key_pressed");
        window_.remove_handler("game_key_released");
        GameBus::get().off(GameEvent::BulletShot);
        GameBus::get().off(GameEvent::GameRestarted);
        GameBus::get().off(GameEvent::GameEnded);
        GameBus::get().off(GameEvent::GameUnpaused);
        GameBus::get().off(GameEvent::SettingsOpened);
        GameBus::get().off(GameEvent::MainMenuOpened);
        GameBus::get().off(GameEvent::GameStarted);
    }
}

template <typename RTreeType>
void Game<RTreeType>::fire_key_event(sf::Keyboard::Key key, const std::string& suffix) {
    auto events = player_manager_.get_events(key);
    for (auto& [event, player] : events) {
        for (auto& [name, obj] : objects_) {
            if (name != player->name()) {
                types_[obj->type_name()].on_player(event + suffix, player, obj);
            } else {
                types_[obj->type_name()].on_own(event + suffix, obj);
            }
        }
    }
}

template <typename RTreeType>
void Game<RTreeType>::start() {
    if (status_ == Status::MainMenu) {
        status_ = Status::Running;
    }
}

template <typename RTreeType>
void Game<RTreeType>::to_main_menu() {
    clear();
    status_ = Status::MainMenu;
}

template <typename RTreeType>
Game<RTreeType>::Status Game<RTreeType>::render(float delta_time) {
    if (status_ == Status::Running) {
        update(delta_time);
    }

    draw_with_default_view(bg_);
    draw_objects();

    draw_ui();

    return status_;
}

template <typename RTreeType>
template <typename T>
void Game<RTreeType>::draw_with_default_view(T&& obj) {
    window_.set_default_view();
    obj.draw(window_);
    window_.set_view(level_ ? level_->field().view() : window_.get_view());
}

template <typename RTreeType>
void Game<RTreeType>::add_object_to_rtree(const std::shared_ptr<GameObject>& obj) {
    rtree_.insert(obj->name(), obj->get_bounds());
    if (obj->hitbox()) {
        hitboxes_.insert(obj->name(), obj->hitbox()->get_bounds());
    }
}

template <typename RTreeType>
void Game<RTreeType>::remove_object_from_rtree(const std::shared_ptr<GameObject>& obj) {
    rtree_.remove(obj->name(), obj->get_bounds());
    if (obj->hitbox()) {
        hitboxes_.remove(obj->name(), obj->hitbox()->get_bounds());
    }
}

template <typename RTreeType>
void Game<RTreeType>::add_object(std::shared_ptr<GameObject>&& obj) {
    add_object_to_rtree(obj);
    objects_.emplace(obj->name(), std::move(obj));
}

template <typename RTreeType>
void Game<RTreeType>::add_objects(std::vector<std::shared_ptr<GameObject>>&& objs) {
    for (auto& obj : objs) {
        add_object(std::move(obj));
    }
}

template <typename RTreeType>
void Game<RTreeType>::draw_objects() {
    if (!level_) {
        return;
    }

    level_->field().draw(window_);

    for (const auto& [_, obj] : objects_) {
        obj->draw(window_);
    }
}

template <typename RTreeType>
void Game<RTreeType>::check_collisions() {
    for (auto obj_hitbox = hitboxes_.begin(); obj_hitbox != hitboxes_.end(); ++obj_hitbox) {
        for (auto subj_hitbox = hitboxes_.intersects(obj_hitbox->first); subj_hitbox != hitboxes_.qend();
             ++subj_hitbox) {
            if (obj_hitbox->second == subj_hitbox->second) {
                continue;
            }
            auto obj = objects_[obj_hitbox->second];
            auto subj = objects_[subj_hitbox->second];

            types_[obj->type_name()].collision(subj->tag(), subj, obj);
        }
    }
}

template <typename RTreeType>
void Game<RTreeType>::resolve_timed_actions(float delta_time) {
    for (auto& [_, obj] : objects_) {
        if (!obj->is_active()) {
            continue;
        }
        obj->resolve_timed_actions(delta_time);
    }
}

template <typename RTreeType>
void Game<RTreeType>::update(float delta_time) {
    update_level();

    generate_objects();

    level_->field().update(delta_time);

    for (auto& [_, obj] : objects_) {
        remove_object_from_rtree(obj);
        obj->update(level_->field(), delta_time);
        add_object_to_rtree(obj);
    }

    check_collisions();
    resolve_timed_actions(delta_time);
    action::Queue::get().resolve(delta_time);

    clear_dead();
    if (player_manager_.count() == 0) {
        status_ = Status::GameOver;
    }

    assets_.clear_unused();
}

template <typename RTreeType>
void Game<RTreeType>::zoom(float value) {
    if (!level_) {
        return;
    }

    level_->field().zoom(value);
}

template <typename RTreeType>
void Game<RTreeType>::update_level() {
    if (level_ && !level_->has_ended()) {
        return;
    }

    auto res = levels_.start_next(window_);
    if (!res) {
        throw res.error();
    }

    level_ = res.value();

    clear();

    return generate_players();
}

template <typename RTreeType>
void Game<RTreeType>::generate_players() {
    menu_.clear();
    player_manager_.clear();

    auto players = player_manager_.load_players(assets_, level_->field(), types_);

    for (auto&& player : players) {
        add_player(std::move(player));
    }
}

template <typename RTreeType>
void Game<RTreeType>::add_player(std::shared_ptr<GameObject>&& player) {
    menu_.add_player(*player, assets_);
    add_object(std::move(player));
}

template <typename RTreeType>
std::shared_ptr<GameObject> Game<RTreeType>::generate_object(const ObjectOptions& opts) {
    if (!types_.contains(opts.type)) {
        throw std::runtime_error(fmt::format("Object type '{}' not found", opts.type));
    }

    return types_.at(opts.type).create_object(opts, assets_);
}

template <typename RTreeType>
void Game<RTreeType>::generate_objects() {
    if (!level_) {
        throw Error("No level loaded");
    }

    while (!level_->object_props().empty()) {
        const auto& opts = level_->object_props().front();

        if (level_->field().view_top() - opts.pos_y > GameObject::kLoadDelta) {
            break;
        }

        auto res = generate_object(opts);
        objects_.emplace(res->name(), std::move(res));
        level_->object_props().pop_front();
    }
}

template <typename RTreeType>
void Game<RTreeType>::clear_dead() {
    std::erase_if(objects_, [this](auto& el) {
        if (!el.second->is_alive()) {
            GameState::get().emit(GameState::Event::ObjectDestroyed, *el.second);

            if (el.second->tag() == GameObjectTag::Player) {
                size_t id = el.second->props().get(kPlayerNum);
                player_manager_.erase_player(id);
                menu_.erase_player(id);
            }

            for (auto& [_, obj] : objects_) {
                types_[obj->type_name()].on_death(el.second->tag(), el.second, obj);
            }

            remove_object_from_rtree(el.second);
            return true;
        }
        return false;
    });
}

template <typename RTreeType>
void Game<RTreeType>::clear() {
    objects_.clear();
    rtree_.clear();
    hitboxes_.clear();
    player_manager_.clear();
    GameState::get().reset();
    for (auto& [_, type] : types_) {
        type.reset_count();
    }

    status_ = Status::Running;
}

template <typename RTreeType>
void Game<RTreeType>::reset() {
    clear();
    levels_.reset();
    level_ = nullptr;
    status_ = Status::MainMenu;
}

template <typename RTreeType>
void Game<RTreeType>::draw_ui() {
    switch (status_) {
        case Status::Settings:
            [[fallthrough]];
        case Status::MainMenu:
            window_.set_default_view();
            main_menu_.draw(window_);
            return;
        case Status::GameOver:
            draw_with_default_view(game_over_);
            break;
        case Status::Paused:
            draw_with_default_view(pause_menu_);
            break;
        default:
            break;
    }

#ifdef DEBUG
    // clang-format off
    ui::StatBox::draw("Debug",
        ui::StatLine{"Objects active", &objects_},
        ui::StatLine{"Objects not loaded", &(level_->object_props())},
        ui::StatLine{"View pos", level_->field().view().getCenter().y},
        ui::StatLine{"Textures loaded", &assets_.textures().storage()},
        ui::StatLine{"Enemies left", GameState::get().enemy_count()}
    );
    // clang-format on
#endif

    menu_.update(player_manager_.players());
    menu_.draw(window_);
}
}  // namespace engine

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "ObjectTypeFactory.hpp"
#include "Utils.hpp"

#ifdef DEBUG
#include "ui/elements/StatBox.hpp"
#endif

namespace engine {

template <typename RTreeType>
Game<RTreeType>::Game(Window& window, SpriteObject&& bg, SideMenu&& menu, PlayerLoader&& player_loader,
                      assets::Manager&& assets, ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps)
    : window_(window),
      bg_(std::move(bg)),
      menu_(std::move(menu)),
      assets_(std::move(assets)),
      types_(std::move(types)),
      levels_(std::move(levels)),
      fps_(fps),
      player_loader_(std::move(player_loader)) {}

template <typename RTreeType>
void Game<RTreeType>::render(float delta_time) {
    update(delta_time);

    draw_with_default_view(bg_);
    draw_objects();

    draw_ui();
}

template <typename RTreeType>
void Game<RTreeType>::draw_with_default_view(Drawable& obj) {
    window_.set_default_view();
    obj.draw(window_);
    window_.set_view(level_->field().view());
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
    for (auto obj = hitboxes_.begin(); obj != hitboxes_.end(); ++obj) {
        for (auto subj = hitboxes_.intersects(obj->first); subj != hitboxes_.qend(); ++subj) {
            if (obj->second == subj->second) {
                continue;
            }
            objects_[obj->second]->resolve_collision(*objects_[subj->second]);
        }
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

    clear_dead();

    assets_.clear_unused();
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

    objects_.clear();

    return generate_players();
}

template <typename RTreeType>
void Game<RTreeType>::generate_players() {
    menu_.clear();
    GameState::get().clear_players();

    auto players = player_loader_.load_players(assets_, level_->field(), types_);

    for (auto&& player : players) {
        add_player(std::move(player));
    }
}

template <typename RTreeType>
void Game<RTreeType>::add_player(std::shared_ptr<GameObject>&& player) {
    GameState::get().add_player(player);
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

    while (!level_->objects().empty()) {
        const auto& opts = level_->objects().front();

        if (level_->field().view_top() - opts.pos_y > GameObject::kLoadDelta) {
            break;
        }

        auto res = generate_object(opts);
        objects_.emplace(res->name(), std::move(res));
        level_->objects().pop_front();
    }
}

template <typename RTreeType>
void Game<RTreeType>::clear_dead() {
    std::erase_if(objects_, [this](auto& el) {
        if (!el.second->is_alive()) {
            GameState::get().emit(GameState::Event::ObjectDestroyed, el.second->tag());
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
    GameState::get().reset();
    for (auto& [_, type] : types_) {
        type.reset_count();
    }
}

template <typename RTreeType>
void Game<RTreeType>::draw_ui() {
#ifdef DEBUG
    // clang-format off
    ui::StatBox::draw("Debug",
        ui::StatLine{"Objects active", &objects_},
        ui::StatLine{"Objects not loaded", &(level_->objects())},
        ui::StatLine{"View pos", level_->field().view().getCenter().y},
        ui::StatLine{"Textures loaded", &assets_.textures().storage()},
        ui::StatLine{"Enemies left", GameState::get().enemy_count()}
    );
    // clang-format on
#endif

    menu_.update(GameState::get().players());
    menu_.draw(window_);
}
}  // namespace engine

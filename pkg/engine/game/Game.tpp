#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "ObjectTypeFactory.hpp"
#include "Utils.hpp"
#include "ui/elements/StatBox.hpp"

namespace engine {

template <typename RTreeType>
Game<RTreeType>::Game(Window& window, PlayerList&& players, AssetManager<sf::Texture>&& textures,
                      AssetManager<sf::SoundBuffer>&& sounds, ObjectTypeFactory::res_type&& types,
                      LevelManager&& levels, int fps)
    : window_(window),
      textures_(std::move(textures)),
      sounds_(std::move(sounds)),
      types_(std::move(types)),
      player_types_(std::move(players)),
      levels_(std::move(levels)),
      fps_(fps) {}

template <typename RTreeType>
Error Game<RTreeType>::render(float delta_time) {
    auto err = update(delta_time);
    if (err) {
        return err;
    }

    draw_objects();
    draw_ui();

    return Error::OK;
}

template <typename RTreeType>
void Game<RTreeType>::set_object_pos(GameObject& obj, const sf::Vector2f& pos) {
    rtree_.remove(obj.name(), obj.get_bounds());
    obj.set_pos(pos);
    rtree_.insert(obj.name(), obj.get_bounds());
}

template <typename RTreeType>
void Game<RTreeType>::draw_objects() {
    if (!level_) {
        return;
    }

    level_->field().draw(window_);

    for (const auto& [_, obj] : objects_) {
        obj.draw(window_);
    }
}

template <typename RTreeType>
Error Game<RTreeType>::update(float delta_time) {
    auto err = update_level();
    if (err) {
        return err;
    }

    err = generate_objects();
    if (err) {
        return err;
    }

    level_->field().update(delta_time);

    for (auto& [_, obj] : objects_) {
        rtree_.remove(obj.name(), obj.get_bounds());
        obj.update(level_->field(), delta_time);
        rtree_.insert(obj.name(), obj.get_bounds());
    }

    clear_dead();

    textures_.storage().clear_unused();
    sounds_.storage().clear_unused();

    return Error::OK;
}

template <typename RTreeType>
Error Game<RTreeType>::update_level() {
    if (level_ && !level_->has_ended()) {
        return Error::OK;
    }

    objects_.clear();

    auto res = levels_.start_next(window_, textures_);
    if (!res) {
        return res.error();
    }

    level_ = res.value();
    return generate_players();
}

template <typename RTreeType>
ErrorOr<GameObject> Game<RTreeType>::generate_object(const ObjectOptions& opts) {
    if (!types_.contains(opts.type)) {
        return Error::New(fmt::format("Object type '{}' not found", opts.type));
    }

    auto obj = types_.at(opts.type).create_object(opts, textures_);
    if (!obj) {
        return tl::unexpected(obj.error());
    }

    if (!level_) {
        return Error::New("No level loaded");
    }

    return obj;
}

template <typename RTreeType>
Error Game<RTreeType>::generate_objects() {
    if (!level_) {
        return Error("No level loaded");
    }

    while (!level_->objects().empty()) {
        const auto& opts = level_->objects().front();

        if (level_->field().view_top() - opts.pos_y > GameObject::kLoadDelta) {
            break;
        }

        auto res = generate_object(opts);
        if (!res) {
            return res.error();
        }

        objects_.emplace(res->name(), std::move(*res));
        level_->objects().pop_front();
    }

    return Error::OK;
}

template <typename RTreeType>
void Game<RTreeType>::clear_dead() {
    std::erase_if(objects_, [](auto& el) {
        if (!el.second.is_alive()) {
            GameState::get().emit(GameState::Event::ObjectDestroyed, el.second.tag());
            return true;
        }
        return false;
    });
}

template <typename RTreeType>
Error Game<RTreeType>::generate_players() {
    size_t idx = 1;
    for (auto& [gen, opts] : player_types_) {
        auto player = gen.create_player(textures_, level_->field(), opts);
        if (!player) {
            return player.error();
        }
        objects_.emplace(player->name(), std::move(*player));
        ++idx;
    }

    return Error::OK;
}

template <typename RTreeType>
void Game<RTreeType>::draw_ui() {
    // clang-format off
    ui::StatBox::draw("Debug",
        ui::StatLine{"Objects active", &objects_},
        ui::StatLine{"Objects not loaded", &(level_->objects())},
        ui::StatLine{"View pos", level_->field().view().getCenter().y},
        ui::StatLine{"Textures loaded", &textures_.storage()},
        ui::StatLine{"Enemies left", GameState::get().enemy_count()}
    );
    // clang-format on
}
}  // namespace engine

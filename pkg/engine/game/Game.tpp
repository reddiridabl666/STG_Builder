#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "ObjectTypeFactory.hpp"
#include "Utils.hpp"
#include "ui/elements/StatBox.hpp"

namespace engine {

static constexpr const char* kPlayerNum = "__player_num";

template <typename RTreeType>
Game<RTreeType>::Game(Window& window, SpriteObject&& bg, PlayerLoader&& player_loader,
                      assets::Textures&& textures, assets::Sounds&& sounds,
                      ObjectTypeFactory::res_type&& types, LevelManager&& levels, int fps)
    : window_(window),
      bg_(std::move(bg)),
      textures_(std::move(textures)),
      sounds_(std::move(sounds)),
      types_(std::move(types)),
      levels_(std::move(levels)),
      fps_(fps),
      player_loader_(std::move(player_loader)) {}

template <typename RTreeType>
Error Game<RTreeType>::render(float delta_time) {
    auto err = update(delta_time);
    if (err) {
        return err;
    }

    draw_with_default_view(bg_);
    draw_objects();
    draw_ui();

    return Error::OK;
}

template <typename RTreeType>
void Game<RTreeType>::draw_with_default_view(Drawable& obj) {
    window_.set_default_view();
    obj.draw(window_);
    window_.set_view(level_->field().view());
}

template <typename RTreeType>
void Game<RTreeType>::add_object(GameObject&& obj) {
    rtree_.insert(obj.name(), obj.get_bounds());
    objects_.emplace(obj.name(), std::move(obj));
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

    auto res = levels_.start_next(window_);
    if (!res) {
        return res.error();
    }

    level_ = res.value();

    objects_.clear();

    return generate_players();
}

template <typename RTreeType>
Error Game<RTreeType>::generate_players() {
    auto players = player_loader_.load_players(textures_, level_->field(), types_);
    if (!players) {
        return players.error();
    }

    for (auto&& player : *players) {
        int id = std::stoi(player.name().substr(player.name().rfind('-')));
        player.props().set(kPlayerNum, id);

        rtree_.insert(player.name(), FloatBox(player.get_bounds()));
        objects_.emplace(player.name(), std::move(player));
    }
    return Error::OK;
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
void Game<RTreeType>::clear() {
    objects_.clear();
    rtree_.clear();
    GameState::get().reset();
    for (auto& [_, type] : types_) {
        type.reset_count();
    }
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

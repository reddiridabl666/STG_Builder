#include "App.hpp"

#include "GameInfo.hpp"
#include "StatBox.hpp"
#include "Utils.hpp"

App::App(Window& window, AssetManager<sf::Texture>&& textures, AssetManager<sf::SoundBuffer>&& sounds,
         ObjectTypeFactory::res_type&& types, LevelManager&& levels)
    : window_(window),
      textures_(std::move(textures)),
      sounds_(std::move(sounds)),
      types_(std::move(types)),
      levels_(std::move(levels)) {}

ErrorPtr App::run() {
    // ui_elements_.push_back(std::make_shared<Test>());

    sf::Clock timer;

    while (window_.is_open()) {
        auto err = update(timer.restart().asSeconds());
        if (err) {
            return err;
        }

        window_.process_events();
        window_.clear();

        draw_objects();
        draw_ui();

        window_.display();
    }

    return ErrorPtr::OK;
}

void App::draw_objects() {
    level_->field().draw(window_);

    for (const auto& [_, obj] : objects_) {
        obj.draw(window_);
    }
}

ErrorPtr App::update(float delta_time) {
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
        obj.update(level_->field(), delta_time);
    }

    clear_dead();

    textures_.storage().clear_unused();
    sounds_.storage().clear_unused();

    return ErrorPtr::OK;
}

ErrorPtr App::update_level() {
    if (level_ && !level_->has_ended()) {
        return ErrorPtr::OK;
    }

    auto res = levels_.start_next(window_, textures_);
    if (!res) {
        return res.error();
    }

    level_ = res.value();
    return ErrorPtr::OK;
}

ErrorOr<GameObject> App::generate_object(const ObjectOptions& opts) {
    if (!types_.contains(opts.type)) {
        return unexpected_error<InternalError>(fmt::format("Object type '{}' not found", opts.type));
    }

    auto obj = types_.at(opts.type).create_object(textures_);
    if (!obj) {
        return tl::unexpected(obj.error());
    }

    if (!level_) {
        return unexpected_error<InternalError>("No level loaded");
    }

    opts.set_props(*obj);
    return obj;
}

ErrorPtr App::generate_objects() {
    if (!level_) {
        return make_error<InternalError>("No level loaded");
    }

    while (!level_->objects().empty()) {
        const auto& opts = level_->objects().front();

        if (level_->field().view_top() - opts.pos_y > GameObject::kLoadDelta) {
            // fmt::println("Obj '{}' with pos {} is higher than {}, does not need to be loaded yet",
            // opts.type, opts.pos_y, level_->field().view_top());
            break;
        }

        auto res = generate_object(opts);
        if (!res) {
            return res.error();
        }

        objects_.emplace(res->name(), std::move(*res));
        level_->objects().pop_front();
    }

    return ErrorPtr::OK;
}

void App::clear_dead() {
    std::erase_if(objects_, [](auto& el) {
        if (!el.second.is_alive()) {
            Game::info().emit(Game::Event::ObjectDestroyed, el.second.tag());
            return true;
        }
        return false;
    });
}

void App::draw_ui() {
    window_.update_ui();

    // clang-format off
    StatBox::draw("Debug",
        StatLine{"Objects active", &objects_},
        StatLine{"Objects not loaded", &(level_->objects())},
        StatLine{"View pos", level_->field().view().getCenter().y},
        StatLine{"Textures loaded", &textures_.storage()},
        StatLine{"Enemies left", Game::info().enemy_count()}
    );
    // clang-format on
}

#include "App.hpp"

#include "GameInfo.hpp"

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
        auto err = update(timer.restart().asMicroseconds());
        if (err) {
            return err;
        }

        draw_objects();

        // window_.draw_ui(ui_elements_);

        window_.process_events();
        window_.display();
    }

    return ErrorPtr::OK;
}

void App::draw_objects() {
    for (const auto& obj : objects_) {
        window_.draw(obj);
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

    for (auto& obj : objects_) {
        obj.update(delta_time);
    }

    return ErrorPtr::OK;
}

ErrorPtr App::update_level() {
    if (level_ && !level_->has_ended()) {
        return ErrorPtr::OK;
    }

    auto res = levels_.start_next(textures_);
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

static constexpr float kLoadDeltaY = 100;

ErrorPtr App::generate_objects() {
    if (!level_) {
        return make_error<InternalError>("No level loaded");
    }

    auto it = level_->objects().begin();

    while (it->pos_y - level_->field().top() > kLoadDeltaY) {
        auto res = generate_object(*it);
        if (!res) {
            return res.error();
        }
        objects_.push_back(std::move(*res));
    }
}

#include "EditableGame.hpp"

namespace builder {
void EditableGame::render_debug() {
    draw_objects();
    rtree_.draw(window_);
}

void EditableGame::reload_objects() {
    if (!level_) {
        return;
    }

    clear();

    size_t idx = 0;
    for (auto& opts : level_->objects()) {
        auto obj = generate_object_debug(idx, opts);
        if (!obj) {
            throw std::runtime_error(fmt::format("error generating objects: {}", obj.error().message()));
        }

        ++idx;

        add_object(std::move(*obj));
    }

    auto err = generate_players();
    if (err) {
        throw std::runtime_error(fmt::format("error generating players: {}", err.message()));
    }
}

GameObject* EditableGame::get_object(const sf::Vector2f& pos) {
    auto it = rtree_.contains(pos);
    if (it != rtree_.qend()) {
        return &objects_.at(it->second);
    }
    return nullptr;
}

GameObject* EditableGame::get_object(const std::string& name) {
    auto it = objects_.find(name);
    if (it == objects_.end()) {
        return nullptr;
    }
    return &(it->second);
}

void EditableGame::set_object_pos(GameObject& obj, const sf::Vector2f& pos) {
    rtree_.remove(obj.name(), obj.get_bounds());
    obj.set_pos(pos);
    rtree_.insert(obj.name(), obj.get_bounds());
}

inline ErrorOr<GameObject> EditableGame::generate_object_debug(size_t idx, const ObjectOptions& opts) {
    auto obj = generate_object(opts);
    if (!obj) {
        return obj;
    }
    obj->props().set(kOptsID, idx);
    obj->props().set(kJsonID, opts.json_id);
    return obj;
}

void EditableGame::scroll(float value) {
    if (!level_) {
        return;
    }

    scrolled_ += value;

    level_->field().move_view(sf::Vector2f{0, value});
}

void EditableGame::scroll_back() {
    scroll(-1 * scrolled_);
}

void EditableGame::move_view(const sf::Vector2f& vec) {
    if (!level_) {
        return;
    }

    level_->field().move_view(vec);
}

void EditableGame::zoom(float value) {
    if (!level_) {
        return;
    }

    level_->field().zoom(value);
}

GameObject& EditableGame::new_object(const std::string& type) {
    auto it = types_.find(type);
    if (it == types_.end()) {
        throw std::runtime_error(fmt::format("No such object type {}", type));
    }

    ObjectOptions opts(type, window_.get_view().getCenter());

    auto obj = it->second.create_object(opts, textures_);
    if (!obj) {
        throw std::runtime_error(fmt::format("Error creating object: {}", obj.error().message()));
    }

    auto obj_name = obj->name();
    obj->props().set(kOptsID, level_->objects().size());

    add_object(std::move(*obj));

    level_->objects().push_back(std::move(opts));
    level_->prepare_objects();
    return objects_.at(obj_name);
}

void EditableGame::new_object_type(const std::string& type) {
    types_[type] = ObjectType{type, kInitSize};
}

void EditableGame::remove_object(const std::string& name) {
    const auto& obj = objects_.at(name);
    rtree_.remove(name, obj.get_bounds());
    level_->objects().erase(level_->objects().begin() + obj.props().at(kOptsID));
    objects_.erase(name);
}

GameObject& EditableGame::reload_object(const std::string& name, const ObjectOptions& opts) {
    const auto& obj = objects_.at(name);
    rtree_.remove(name, obj.get_bounds());

    size_t id = obj.props().at(kOptsID);
    level_->objects()[id] = opts;
    objects_.erase(name);

    auto new_obj = generate_object_debug(id, opts);
    std::string new_name = new_obj->name();

    add_object(std::move(*new_obj));
    return objects_.at(new_name);
}

Error EditableGame::prepare_preview(size_t level) {
    auto err = choose_level(level);
    if (err) {
        return err;
    }
    return generate_players();
}

Error EditableGame::choose_level(size_t num) {
    auto res = levels_.get(num, window_);
    if (!res) {
        return res.error();
    }

    level_ = res.value();
    return Error::OK;
}

EditableGame::~EditableGame() {
    GameState::get().reset();
}
}  // namespace builder

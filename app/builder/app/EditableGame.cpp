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

    objects_.clear();

    size_t idx = 0;
    for (auto& opts : level_->objects()) {
        auto obj = generate_object_debug(idx, opts);
        if (!obj) {
            throw std::runtime_error("Error generating objects");
        }

        ++idx;

        rtree_.insert(obj->name(), FloatBox(obj->get_bounds()));
        objects_.emplace(obj->name(), std::move(*obj));
    }
}

GameObject* EditableGame::object_by_pos(const sf::Vector2f& pos) {
    auto it = rtree_.contains(pos);
    if (it != rtree_.qend()) {
        return &objects_.at(it->second);
    }
    return nullptr;
}

size_t EditableGame::object_count(const std::string& type) const {
    return std::ranges::count_if(objects_, [&type](const auto& el) {
        return el.first.substr(0, el.first.rfind('-')) == type;
    });
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

    level_->field().move_view(sf::Vector2f{0, value});
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
    rtree_.insert(obj_name, obj->get_bounds());
    obj->props().set(kOptsID, level_->objects().size());

    objects_.emplace(obj_name, std::move(*obj));

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

Error EditableGame::choose_level(size_t num) {
    objects_.clear();

    auto res = levels_.get(num, window_, textures_);
    if (!res) {
        return res.error();
    }

    level_ = res.value();
    return generate_players();
}
}  // namespace builder

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

        // rtree_.insert(obj->name(), FloatBox(obj->get_bounds()));
        rtree_.insert(std::pair(FloatBox(obj->get_bounds()), obj->name()));
        objects_.emplace(obj->name(), std::move(*obj));
    }

    // for (auto& [box, _] : rtree_) {
    //     fmt::println("{{{}, {}, {}, {}}}", box.top_left().x, box.top_left().y, box.bottom_right().x,
    //                  box.bottom_right().y);
    // }
}

GameObject* EditableGame::object_by_pos(const sf::Vector2f& pos) {
    // fmt::println("Pos: {{{}, {}}}", pos.x, pos.y);

    auto it = rtree_.contains(pos);
    if (it != rtree_.qend()) {
        return &objects_.at(it->second);
    }
    return nullptr;
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

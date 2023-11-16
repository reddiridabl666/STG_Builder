#include "GameState.hpp"

#include <algorithm>

#include "GameObject.hpp"

size_t GameState::enemy_count() {
    return objects_by_tag(GameObjectTag::Enemy).size();
}

void GameState::reset() {
    objects_.clear();
}

const std::vector<std::weak_ptr<const GameObject>>& GameState::objects_by_tag(GameObjectTag tag) {
    auto& objects = objects_[tag];

    std::erase_if(objects, [](const auto& obj) {
        return obj.expired();
    });

    return objects;
}

GameState::GameState() {
    on(Event::ObjectCreated, [this](const auto& obj) {
        objects_[obj.tag()].push_back(obj.weak_from_this());
    });
}

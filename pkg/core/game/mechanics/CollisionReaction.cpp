#include "CollisionReaction.hpp"

#include "ActionFactory.hpp"
#include "GameObject.hpp"

void CollisionReaction::operator()(GameObject& subject, GameObject& object) const {
    auto action = actions_.find(subject.tag());
    if (action == actions_.end() || !action->second) {
        return;
    }
    action->second->operator()(subject, object);
}

void from_json(const nl::json& json, CollisionReaction& collision) {
    for (auto& [tag, action] : json.items()) {
        collision.actions_[nl::json(tag).get<GameObjectTag>()] = action::Factory::create(action);
    }
}

void to_json(nl::json& json, const CollisionReaction& collision) {
    for (auto& [tag, action] : collision.actions_) {
        json[GameObject::tag_to_str(tag)] = action->to_json();
    }
}

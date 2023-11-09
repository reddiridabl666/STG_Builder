#pragma once

#include "Action.hpp"
#include "ActionFactory.hpp"

namespace action {

template <typename Key, typename ActionType>
struct Map;

template <typename Key, typename ActionType>
void from_json(const nl::json&, Map<Key, ActionType>&);

template <typename Key, typename ActionType>
void to_json(nl::json&, const Map<Key, ActionType>&);

template <typename Key, typename ActionType = Action>
struct Map {
  public:
    void operator()(const Key& key, const GameObject& subject, GameObject& object) const
        requires std::is_same_v<ActionType, BinaryAction>;

    void operator()(const Key& key, GameObject& object) const;

  private:
    std::unordered_map<Key, std::unique_ptr<ActionType>> actions_;

    friend void from_json<Key, ActionType>(const nl::json&, Map<Key, ActionType>&);
    friend void to_json<Key, ActionType>(nl::json&, const Map<Key, ActionType>&);
};

template <typename Key, typename ActionType>
inline void Map<Key, ActionType>::operator()(const Key& key, const GameObject& subject, GameObject& object) const
    requires std::is_same_v<ActionType, action::BinaryAction>
{
    auto action = actions_.find(key);
    if (action == actions_.end() || !action->second) {
        return;
    }
    action->second->operator()(subject, object);
}

template <typename Key, typename ActionType>
inline void from_json(const nl::json& json, Map<Key, ActionType>& collision) {
    for (auto& [key, action] : json.items()) {
        collision.actions_[nl::json(key).get<Key>()] = action::Factory::create<ActionType>(action);
    }
}

template <typename Key, typename ActionType>
inline void to_json(nl::json& json, const Map<Key, ActionType>& collision) {
    for (auto& [key, action] : collision.actions_) {
        json[nl::json(key).get<std::string>()] = action->to_json();
    }
}

template <typename Key, typename ActionType>
inline void Map<Key, ActionType>::operator()(const Key& key, GameObject& object) const {
    auto action = actions_.find(key);
    if (action == actions_.end() || !action->second) {
        return;
    }
    action->second->operator()(object);
}
}  // namespace action

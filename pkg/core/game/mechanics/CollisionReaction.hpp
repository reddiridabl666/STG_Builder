#pragma once

#include <unordered_map>

#include "Action.hpp"
#include "GameObjectTag.hpp"

class GameObject;

struct CollisionReaction {
  public:
    void operator()(GameObject& subject, GameObject& object) const;

  private:
    std::unordered_map<GameObjectTag, std::unique_ptr<action::Action>> actions_;

    friend void from_json(const nl::json&, CollisionReaction&);
    friend void to_json(nl::json&, const CollisionReaction&);
};

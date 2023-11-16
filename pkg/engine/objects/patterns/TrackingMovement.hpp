#pragma once

#include <SFML/System/Vector2.hpp>

#include "FindNearest.hpp"
#include "GameObject.hpp"
#include "GameState.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct TrackingMovement : Pattern::MovementSetter {
  public:
    TrackingMovement(GameObjectTag tag) : tag_(tag) {}

    void set(const GameObject&, Pattern::objects& objects) const override {
        auto targets = GameState::get().objects_by_tag(tag_);
        for (auto& obj : objects) {
            auto target = find_nearest(obj, targets);
            obj->set_movement(movement::tracking(target));
        }
    }

  private:
    const GameObjectTag tag_;
};

#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct FollowingParentMovement : Pattern::MovementSetter {
    void set(const GameObject&, Pattern::objects& objects) const override {
        for (auto& obj : objects) {
            obj->set_movement(movement::following_parent());
        }
    }
};

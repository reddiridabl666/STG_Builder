#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "LinAlg.hpp"
#include "Math.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct FromCenterMovement : Pattern::MovementSetter {
    void set(const GameObject& parent, Pattern::objects& objects) const override {
        for (auto& obj : objects) {
            auto direction = linalg::unit(obj->pos() - parent.pos());
            obj->set_movement(movement::linear(direction));
            obj->set_rotation(math::to_degrees(std::atan(-direction.x / direction.y)));
        }
    }
};

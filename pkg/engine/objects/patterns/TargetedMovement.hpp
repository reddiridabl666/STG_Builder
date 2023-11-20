#pragma once

#include <SFML/System/Vector2.hpp>

#include "FindNearest.hpp"
#include "GameObject.hpp"
#include "GameState.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct TargetedMovement : Pattern::MovementSetter {
  public:
    TargetedMovement(GameObjectTag tag) : tag_(tag) {}

    void set(const GameObject&, Pattern::objects& objects) const override {
        auto targets = GameState::get().objects_by_tag(tag_);
        for (auto& obj : objects) {
            auto target = find_nearest(obj, targets);
            auto direction = target.expired() ? sf::Vector2f{0, -1} : linalg::unit(target.lock()->pos() - obj->pos());
            obj->set_movement(movement::linear(direction));
            obj->set_rotation(math::to_degrees(std::atan(-direction.x / direction.y)));
        }
    }

  private:
    const GameObjectTag tag_;
};

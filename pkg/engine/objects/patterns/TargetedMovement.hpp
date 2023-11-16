#pragma once

#include <SFML/System/Vector2.hpp>

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
            obj->set_movement(movement::linear(linalg::unit(target - obj->pos())));
        }
    }

  private:
    static sf::Vector2f find_nearest(const std::shared_ptr<GameObject>& to,
                                     const std::vector<std::weak_ptr<const GameObject>>& objects) {
        float distance = std::numeric_limits<float>::max();
        sf::Vector2f target;

        for (auto& obj : objects) {
            float new_distance = linalg::distance(obj.lock()->pos(), to->pos());
            if (distance > new_distance && obj.lock()->name() != to->name()) {
                distance = new_distance;
                target = obj.lock()->pos();
            }
        }

        return target;
    }

    const GameObjectTag tag_;
};

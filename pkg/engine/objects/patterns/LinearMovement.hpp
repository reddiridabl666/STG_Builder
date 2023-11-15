#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct LinearMovement : Pattern::MovementSetter {
  public:
    LinearMovement(const sf::Vector2f& velocity) : velocity_(velocity) {}

    void set(const GameObject&, Pattern::objects& objects) const override {
        for (auto& obj : objects) {
            obj->set_movement(movement::linear(velocity_));
        }
    }

  private:
    const sf::Vector2f velocity_;
};

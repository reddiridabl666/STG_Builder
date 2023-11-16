#pragma once
#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"
#include "Random.hpp"

struct RandomMovement : Pattern::MovementSetter {
  public:
    RandomMovement(const sf::Vector2f& min, const sf::Vector2f& max)
        : area_(min.x, min.y, max.x - min.x, max.y - min.y) {}

    void set(const GameObject&, Pattern::objects& objects) const override {
        for (auto& obj : objects) {
            auto direction = rng::vec(area_);
            obj->set_movement(movement::linear(direction));
            obj->set_rotation(math::to_degrees(std::atan(-direction.x / direction.y)));
        }
    }

  private:
    const sf::FloatRect area_;
};

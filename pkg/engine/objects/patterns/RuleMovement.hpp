#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "GameState.hpp"
#include "Movement.hpp"
#include "Pattern.hpp"

struct RuleMovement : Pattern::MovementSetter {
  public:
    RuleMovement(std::unique_ptr<movement::Rule>&& rule) : rule_(std::move(rule)) {}

    void set(const GameObject&, Pattern::objects& objects) const override {
        for (auto& obj : objects) {
            obj->set_movement(rule_->clone());
        }
    }

  private:
    std::unique_ptr<movement::Rule> rule_;
};

#pragma once

#include <SFML/System/Vector2.hpp>
#include <numbers>

#include "GameObject.hpp"
#include "Pattern.hpp"

class ArcPosition : public Pattern::PositionSetter {
  public:
    ArcPosition(const sf::Vector2f& offset, float radius, float init_angle = 0, float angle = 2 * std::numbers::pi)
        : offset_(offset), radius_(radius), angle_(angle), init_angle_(init_angle) {}

    void set(const GameObject& parent, Pattern::objects& objects) const override {
        auto init_pos = parent.pos() + offset_;
        float angle = init_angle_;
        float delta_angle = angle_ / (objects.size() - 1);

        for (auto& obj : objects) {
            auto pos = init_pos + radius_ * sf::Vector2f{cos(angle), sin(angle)};
            obj->set_pos(pos);
            angle += delta_angle;
        }
    }

  private:
    const sf::Vector2f offset_;
    const float radius_;
    const float angle_;
    const float init_angle_;
};

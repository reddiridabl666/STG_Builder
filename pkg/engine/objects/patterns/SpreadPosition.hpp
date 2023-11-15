#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Pattern.hpp"
#include "Random.hpp"

class SpreadPosition : public Pattern::PositionSetter {
  public:
    SpreadPosition(const sf::Vector2f& parent_offset, const sf::Vector2f& space)
        : parent_offset_(parent_offset), space_(space) {}

    void set(const GameObject& parent, Pattern::objects& objects) const override {
        auto rect_center = parent.pos() + parent_offset_;

        sf::FloatRect absolute_space{
            rect_center.x - space_.x / 2,
            rect_center.y - space_.y / 2,
            space_.x,
            space_.y,
        };

        for (auto& obj : objects) {
            obj->set_pos(rng::vec(absolute_space));
        }
    }

  private:
    const sf::Vector2f parent_offset_;
    const sf::Vector2f space_;
};

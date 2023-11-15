#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Pattern.hpp"

class RectanglePosition : public Pattern::PositionSetter {
  public:
    RectanglePosition(const sf::Vector2f& parent_offset, const sf::Vector2u& ratio, float sibling_offset)
        : parent_offset_(parent_offset), ratio_(ratio), sibling_offset_(sibling_offset) {}

    void set(const GameObject&, Pattern::objects&) const override {}

  private:
    // static size_t gcd(size_t a, size_t b) {
    //     return b == a ? b : gcd();
    // }

    const sf::Vector2f parent_offset_;
    const sf::Vector2u ratio_;
    const float sibling_offset_;
};

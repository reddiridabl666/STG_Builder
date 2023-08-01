#pragma once

#include "GameObject.hpp"
#include "SpriteObject.hpp"

class GameField : public GameObject {
  public:
    GameField(std::unique_ptr<SpriteObject>&& image, int speed = 50, const Properties::Data& props = {})
        : GameObject(std::move(image), speed, props) {
        set_movement(movement::linear());
    }

    sf::Vector2f center() const {
        return sf::Vector2f{};
    }

    float right() const {
        return 0;
    }

    float left() const {
        return 0;
    }

    float top() const {
        return 0;
    }

    float bottom() const {
        return 0;
    }

    float height() const {
        return 0;
    }

    float width() const {
        return 0;
    }

    // singleton?
};

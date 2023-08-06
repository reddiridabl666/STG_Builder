#pragma once

#include "GameObject.hpp"
#include "SpriteObject.hpp"

class GameField : public GameObject {
  public:
    GameField(std::unique_ptr<SpriteObject>&& image, int speed = 50, const Properties::Data& props = {})
        : GameObject(std::move(image), speed, GameObject::Tag::Background, props) {
        set_movement(movement::linear());
    }

    sf::Vector2f center() const {
        return sf::Vector2f{};
    }

    // TODO: Implement everything...

    float end() const {
        return height();
    }

    // These coordinates are independent of actual game field position,
    // should be calculated somehow using screen width and height

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
        return get_size().y;
    }

    float width() const {
        return get_size().x;
    }
};

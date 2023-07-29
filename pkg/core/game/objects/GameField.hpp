#pragma once

#include "GameObject.hpp"
#include "SpriteObject.hpp"

class GameField : public GameObject {
    GameField(std::unique_ptr<SpriteObject>&& image, int speed = 50, const Properties::Data& props = {})
        : GameObject(std::move(image), speed, props) {
        set_movement(movement::linear());
    }
};

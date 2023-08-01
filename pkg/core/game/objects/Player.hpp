#pragma once

#include "GameObject.hpp"
#include "Movement.hpp"

class Player : public GameObject {
  public:
    Player(std::unique_ptr<Displayable>&& image, int speed = 50, const Properties& props = {},
           int player_num = 1)
        : GameObject(std::move(image), speed, props), player_num_(player) {
        set_movement(movement::user_control(player_num));
    }

    int player_num() {
        return player_num_;
    }

  private:
    int player_num_;
};

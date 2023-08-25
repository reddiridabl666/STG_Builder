#pragma once

#include "Controls.hpp"
#include "GameObject.hpp"
#include "Movement.hpp"

class Player : public GameObject {
  public:
    Player(const sf::Vector2f& size, std::unique_ptr<Displayable>&& image, int speed = 50,
           const Properties& props = {}, int player_num = 1)
        : GameObject(fmt::format("player-{}", player_num), size, std::move(image), speed,
                     GameObject::Tag::Player, props),
          player_num_(player_num) {
        set_movement(movement::user_control(player_num));
    }

    int player_num() {
        return player_num_;
    }

  private:
    int player_num_;
};

struct PlayerOptions {
    int num;
    KeyControls keys = kDefaultKeyControls;
    JoyControls joy = kDefaultJoyControls;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerOptions, keys, joy)

using PlayerList = std::vector<std::pair<ObjectType, PlayerOptions>>;

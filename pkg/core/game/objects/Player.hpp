#pragma once

#include "Controls.hpp"
#include "GameObject.hpp"

// class Player : public GameObject {
//     // clang-format off
//     Player(
//         const std::string& name,
//         const sf::Vector2f& size,
//         std::unique_ptr<Displayable>&& image,
//         int speed = 50,
//         GameObjectTag tag = GameObjectTag::Object,
//         const Properties& props = {},
//         float activity_start = kDefaultActivityStart,
//         const alive::update& life_func = kDefaultLifeFunc,
//         std::unique_ptr<movement::Rule>&& move_func = movement::no_op(),
//         std::unique_ptr<Hitbox>&& hitbox = nullptr,
//         CollisionReaction&& collision = {},

//         std::unique_ptr<action::Action>&& on_character_death = nullptr,
//         bool stop_at_bounds = false,
//         sf::Vector2f velocity = {},
//         bool alive = true,
//         bool active = false
//     );
//     // clang-format on
// };

struct PlayerOptions {
    size_t num;
    KeyControls keys = kDefaultKeyControls;
    JoyControls joy = kDefaultJoyControls;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerOptions, keys, joy)

using PlayerList = std::vector<std::shared_ptr<GameObject>>;

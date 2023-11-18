#pragma once
#include <nlohmann/json.hpp>

enum class GameObjectTag {
    Object,
    Item,
    Enemy,
    Bullet,
    Player,
    PlayerBullet
};

// clang-format off
#define OBJECT_TAG_TO_STRING \
    {GameObjectTag::Object, "object"}, \
    {GameObjectTag::Item, "item"}, \
    {GameObjectTag::Enemy, "enemy"}, \
    {GameObjectTag::Bullet, "bullet"}, \
    {GameObjectTag::Player, "player"}, \
    {GameObjectTag::PlayerBullet, "player_bullet"},

NLOHMANN_JSON_SERIALIZE_ENUM(GameObjectTag, {
    OBJECT_TAG_TO_STRING
})
// clang-format on

#pragma once

#include <nlohmann/json.hpp>

enum class GameEvent {
    ObjectDestroyed,
    BulletCreated
};

// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(GameEvent, {
    {GameEvent::ObjectDestroyed, "object_destroyed"},
})
// clang-format on

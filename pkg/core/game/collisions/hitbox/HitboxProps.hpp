#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Hitbox.hpp"
#include "Json.hpp"

// clang-format off
#define HITBOX_TYPE_TO_STR \
    {Hitbox::Type::Rect, "rect"}, \
    {Hitbox::Type::Circle, "circle"},

NLOHMANN_JSON_SERIALIZE_ENUM(Hitbox::Type, {
    HITBOX_TYPE_TO_STR
})
// clang-format on

namespace sf {
void from_json(const nl::json& json, Color& color);
void to_json(nl::json& json, const Color& color);
}  // namespace sf

struct HitboxProps {
    Hitbox::Type type = Hitbox::Type::Rect;
    sf::Color color = sf::Color::White;
    size_t outline = 1;

    sf::Vector2f size;
    float radius;

    bool hidden = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(HitboxProps, type, color, outline, size, radius)
};

#undef HITBOX_TYPE_TO_STR

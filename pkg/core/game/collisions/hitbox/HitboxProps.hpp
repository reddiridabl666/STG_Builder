#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Hitbox.hpp"
#include "Json.hpp"

// clang-format off
#define HITBOX_TYPE_TO_STR \
    {Hitbox::Type::Rect, "rect"}, \
    {Hitbox::Type::Circle, "circle"}, \
    {Hitbox::Type::No, "none"},

NLOHMANN_JSON_SERIALIZE_ENUM(Hitbox::Type, {
    HITBOX_TYPE_TO_STR
})
// clang-format on

struct HitboxProps {
    Hitbox::Type type = Hitbox::Type::No;

    sf::Color fill_color = sf::Color::Transparent;
    sf::Color outline_color = sf::Color::White;

    int outline = 1;

    // union {
    sf::Vector2f size;
    float radius = 0;
    // };

#ifdef DEBUG
    bool shown = true;
#else
    bool shown = false;
#endif
};

void from_json(const nl::json& json, HitboxProps& hitbox);
void to_json(nl::json& json, const HitboxProps& hitbox);

#undef HITBOX_TYPE_TO_STR

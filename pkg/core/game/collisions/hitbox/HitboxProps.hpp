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

    sf::Color fill_color = sf::Color::Transparent;
    sf::Color outline_color = sf::Color::White;

    int outline = 1;

    sf::Vector2f size;
    float radius;

#ifdef DEBUG
    bool shown = true;
#else
    bool shown = false;
#endif

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(HitboxProps, type, fill_color, outline_color, outline, size, radius,
                                                shown)
};

void from_json(const nl::json& json, HitboxProps& hitbox);
void to_json(nl::json& json, const HitboxProps& hitbox);

#undef HITBOX_TYPE_TO_STR

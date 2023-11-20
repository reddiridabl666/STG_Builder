#include "HitboxProps.hpp"

void from_json(const nl::json& json, HitboxProps& hitbox) {
    if (json.is_null()) {
        return;
    }

    hitbox.type = json.value("type", Hitbox::Type::No);
    hitbox.shown = json.value("shown", hitbox.shown);

    switch (hitbox.type) {
        case Hitbox::Type::Rect:
            hitbox.size = json.value("size", sf::Vector2f{});
            break;
        case Hitbox::Type::Circle:
            hitbox.radius = json.value("radius", 0);
            break;
        default:
            hitbox.shown = false;
            return;
    }

    hitbox.outline = json.value("outline", hitbox.outline);
    hitbox.outline_color = json.value("outline_color", hitbox.outline_color);
    hitbox.fill_color = json.value("fill_color", hitbox.fill_color);
}

void to_json(nl::json& json, const HitboxProps& hitbox) {
    switch (hitbox.type) {
        case Hitbox::Type::Rect:
            json["size"] = hitbox.size;
            break;
        case Hitbox::Type::Circle:
            json["radius"] = hitbox.radius;
            break;
        default:
            return;
    }

    if (hitbox.outline != 1) {
        json["outline"] = hitbox.outline;
    }

    if (hitbox.outline_color != sf::Color::White) {
        json["outline_color"] = hitbox.outline_color;
    }

    if (hitbox.fill_color != sf::Color::Transparent) {
        json["fill_color"] = hitbox.fill_color;
    }

    json["shown"] = hitbox.shown;
    json["type"] = hitbox.type;
}

#include "HitboxProps.hpp"

namespace sf {
void from_json(const nl::json& json, Color& color) {
    static const std::unordered_map<std::string, Color> colors = {
        {"red", Color::Red},       {"green", Color::Green},     {"blue", Color::Blue},
        {"yellow", Color::Yellow}, {"magenta", Color::Magenta}, {"cyan", Color::Cyan},
        {"white", Color::White},   {"black", Color::Black},     {"transparent", Color::Transparent},
    };

    if (json.is_string()) {
        auto it = colors.find(json.get<std::string>());
        if (it == colors.end()) {
            color = Color::Transparent;
        } else {
            color = it->second;
        }
        return;
    }

    color.r = json.value("r", 0);
    color.g = json.value("g", 0);
    color.b = json.value("b", 0);
    color.a = json.value("a", 0);
}

void to_json(nl::json& json, const Color& color) {
    static const std::unordered_map<sf::Uint32, std::string> colors = {
        {Color::Red.toInteger(), "red"},
        {Color::Green.toInteger(), "green"},
        {Color::Blue.toInteger(), "blue"},
        {Color::Yellow.toInteger(), "yellow"},
        {Color::Magenta.toInteger(), "magenta"},
        {Color::Cyan.toInteger(), "cyan"},
        {Color::White.toInteger(), "white"},
        {Color::Black.toInteger(), "black"},
        {Color::Transparent.toInteger(), "transparent"},
    };

    auto it = colors.find(color.toInteger());
    if (it == colors.end()) {
        json["r"] = color.r;
        json["g"] = color.g;
        json["b"] = color.b;
        json["a"] = color.a;
    } else {
        json = it->second;
    }
}
}  // namespace sf

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

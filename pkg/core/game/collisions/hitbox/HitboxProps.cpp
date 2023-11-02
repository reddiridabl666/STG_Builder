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

    color.r = json::get<sf::Uint8>(json, "r");
    color.g = json::get<sf::Uint8>(json, "g");
    color.b = json::get<sf::Uint8>(json, "b");
    color.a = json::get<sf::Uint8>(json, "a");
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

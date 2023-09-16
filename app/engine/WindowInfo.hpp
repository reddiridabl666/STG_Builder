#pragma once

#include "Json.hpp"
#include "Window.hpp"

struct WindowInfo {
    std::string name;
    sf::Vector2i size;

    Window make_window() const {
        return Window(name, size.x, size.y);
    }

    static WindowInfo from_json(const nl::json& json);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowInfo, name, size);

WindowInfo WindowInfo::from_json(const nl::json& json) {
    return json.template get<WindowInfo>();
}

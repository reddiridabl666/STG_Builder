#pragma once

#include "Json.hpp"
#include "Window.hpp"

struct WindowInfo {
    std::string name;
    sf::Vector2u size;

    Window make_window() const {
        return Window(name, size.x, size.y);
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowInfo, name, size);

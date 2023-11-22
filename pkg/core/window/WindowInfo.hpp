#pragma once

#include "Json.hpp"
#include "Window.hpp"

struct WindowInfo {
    std::string name = "Window";
    sf::Vector2u size = {640, 480};

    Window make_window() const {
        return Window(name, size.x, size.y);
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(WindowInfo, name, size);

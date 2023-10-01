#pragma once

#include "Json.hpp"
#include "ui/elements/Menu.hpp"

namespace ui {
Menu::Tab LevelTab(nl::json&, const std::function<void()>& cb);
}  // namespace ui

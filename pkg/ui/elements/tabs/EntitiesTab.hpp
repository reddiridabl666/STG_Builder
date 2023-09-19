#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "Menu.hpp"

namespace ui {
Menu::Tab EntitiesTab(AssetManager<sf::Texture>& textures, nl::json&);
}  // namespace ui

#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "Menu.hpp"

namespace ui {
Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, AssetManager<sf::Texture>& textures, nl::json&);
}  // namespace ui

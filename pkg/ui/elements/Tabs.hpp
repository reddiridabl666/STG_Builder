#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "ui/elements/Menu.hpp"

namespace ui {
Menu::Tab MainTab(nl::json&);

Menu::Tab LevelTab(nl::json&);

Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, AssetManager<sf::Texture>& textures, nl::json&);

Menu::Tab PlayerTab(nl::json&);
}  // namespace ui

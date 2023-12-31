#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "ui/elements/Menu.hpp"

namespace ui {
Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, assets::Textures& textures,
                      nl::json& entities_json, const nl::json& level_objects);
}  // namespace ui

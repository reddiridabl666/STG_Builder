#include "Box.hpp"
#include "GameInfo.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Tabs.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include "EntityEntry.hpp"
#include "EntriesTab.hpp"

namespace ui {
namespace {
using EntitiesTabContents = EntriesTab<EntityEntry, Message::CreateObjectType, "New object {}">;
}  // namespace

Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, AssetManager<sf::Texture>& textures,
                      nl::json& json) {
    Box<EntityEntry>::Items entries;
    entries.reserve(json.size());

    for (auto& [key, val] : json.items()) {
        entries.push_back(std::make_unique<EntityEntry>(
            key, val,
            textures.get_or(game_dir / "assets/images" / json::get<std::string>(val, "image"),
                            kFallbackImage)));
    }

    auto tab = std::make_unique<EntitiesTabContents>(textures, std::move(entries), json);

    return Menu::Tab(std::move(tab), message_func(Message::ObjectTypes));
}
}  // namespace ui

#include <iostream>

#include "Box.hpp"
#include "Controls.hpp"
#include "EntriesTab.hpp"
#include "ImguiUtils.hpp"
#include "Json.hpp"
#include "Messages.hpp"
#include "Tabs.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace ui {

namespace {
struct PlayerControls {
    KeyControls keys = kDefaultKeyControls;
    JoyControls pad = kDefaultJoyControls;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(PlayerControls, keys, pad)
};

struct PlayerInfo : public Element {
  public:
    bool should_delete() {
        return to_delete_;
    };

    nl::json to_json();
    size_t get_name() {
        return id_;
    }

    void draw(const Window&) override;

  private:
    size_t id_;

    std::string image = "";
    std::unordered_map<std::string, int> stats;
    sf::Vector2f size = sf::Vector2f{100, 100};
    float speed = 300;
    PlayerControls opts;

    bool to_delete_ = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(PlayerInfo, image, size, speed, opts)
};

using PlayerTabContents = EntriesTab<PlayerInfo, Message::CreatePlayers, "Player {}">;
}  // namespace

Menu::Tab PlayerTab(const std::filesystem::path& game_dir, AssetManager<sf::Texture>& textures,
                    nl::json& json) {
    Box<PlayerInfo>::Items entries;
    entries.reserve(json.size());

    for (auto& [key, val] : json.items()) {
        entries.push_back(std::make_unique<PlayerInfo>(
            key, val,
            textures.get_or(game_dir / "assets/images" / json::get<std::string>(val, "image"),
                            kFallbackImage)));
    }

    auto tab = std::make_unique<PlayerTabContents>(textures, std::move(entries), json);

    return Menu::Tab(std::move(tab), message_func(Message::Players));
}

}  // namespace ui

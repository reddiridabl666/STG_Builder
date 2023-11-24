#pragma once

#include <memory>
#include <vector>

#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"
#include "StatUi.hpp"
#include "Text.hpp"
#include "Window.hpp"

namespace engine {

struct SideMenuProps {
    sf::Vector2f offset;
    float gap = 25;
    float player_gap = 50;
    std::string bg;
    sf::FloatRect size = {0.65f, 0.05f, 0.3f, 0.9f};
    TextProps player_label = {.text = "Player {}"};
    nl::json stats = nl::json::array();
    float player_label_gap = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(SideMenuProps, offset, gap, player_gap, player_label, player_label_gap,
                                                bg, size, stats)

struct PlayerStat {
    std::string key;
    std::unique_ptr<StatUi> ui;
};

class SideMenu {
  public:
    SideMenu(const Window& window, const sf::Vector2f& offset, float gap, float player_gap,
             const sf::FloatRect& screen_pos, std::shared_ptr<sf::Texture>&& bg, const nl::json& settings,
             const TextProps& player_label, float player_label_gap);

    SideMenu(const Window& window, std::shared_ptr<sf::Texture>&& bg, const SideMenuProps& props);

    void draw(Window& window);

    void update(const PlayerList& players);

    void update_layout(const Window& window, const SideMenuProps& props);
    void update_item(size_t id, const PlayerList& players, assets::Manager& assets, const nl::json& updated);
    void add_item(const PlayerList& players, assets::Manager& assets, const nl::json& item);

    void clear();
    void erase_player(size_t id);
    void erase_item(size_t id);
    void set_bg(std::shared_ptr<sf::Texture>&& bg);

    void add_player(const GameObject&, assets::Manager& assets);

    sf::Vector2f top_left() {
        return bg_.pos();
    }

    struct PlayerStats {
        Text label;
        std::vector<PlayerStat> stats;
    };

  private:
    void initialize_view(const Window& window, const sf::FloatRect& screen_pos);
    void update_layout();

    std::vector<PlayerStats> player_stats_;
    SpriteObject bg_;
    nl::json settings_;
    sf::View view_;

    sf::Vector2f offset_;
    float gap_;
    float player_gap_;
    sf::Vector2f prev_pos_;

    TextProps player_label_props_;
    std::string player_tmpl_;
    float player_label_gap_;
};
}  // namespace engine

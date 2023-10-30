#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "GameUi.hpp"
#include "SpriteObject.hpp"

namespace engine {
class SideMenu {
  public:
    SideMenu(const Window& window, const sf::Vector2f& offset, float gap, float player_gap,
             const sf::FloatRect& screen_pos, std::shared_ptr<sf::Texture>&& bg, const nl::json& settings);

    void draw(Window& window);

    template <typename PlayerList>
    void update(const PlayerList& players) {
        for (size_t i = 0; i < players.size(); ++i) {
            for (auto& [stat_name, stat] : player_stats_[i]) {
                stat->update(players[i]->props().get(stat_name));
            }
        }
    }

    void add_player(const GameObject&, assets::Manager& assets);

    sf::Vector2f top_left() {
        return bg_.pos();
    }

    using PlayerStats = std::unordered_map<std::string, std::unique_ptr<GameUi>>;

  private:
    std::vector<PlayerStats> player_stats_;
    SpriteObject bg_;
    nl::json settings_;
    sf::View view_;

    sf::Vector2f offset_;
    float gap_;
    float player_gap_;
    sf::Vector2f prev_pos_;
};
}  // namespace engine

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "GameUi.hpp"
#include "SpriteObject.hpp"

namespace engine {
class SideMenu {
  public:
    SideMenu(const Window& window, const sf::FloatRect& screen_pos, std::shared_ptr<sf::Texture>&& bg);

    void draw(Window& window);

    void add_player_stat(size_t player_id, const std::string& stat_name, std::unique_ptr<GameUi>&& ui_element);

    sf::Vector2f top_left() {
        return bg_.pos();
    }

  private:
    std::vector<std::unordered_map<std::string, std::unique_ptr<GameUi>>> player_stats_;
    SpriteObject bg_;
    sf::View view_;
};
}  // namespace engine

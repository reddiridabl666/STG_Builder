#pragma once

#include <memory>
#include <unordered_map>

#include "GameObject.hpp"
#include "PlayerLoader.hpp"

class PlayerManager {
  private:
    using EventList = std::vector<std::pair<std::string, std::shared_ptr<GameObject>>>;

  public:
    PlayerManager(PlayerLoader&& loader) : loader_(std::move(loader)) {}

    auto& players() {
        return players_;
    }

    const auto& players() const {
        return players_;
    }

    size_t count() const {
        return players_.size();
    }

    long get_score() const;

    void erase_player(size_t id);
    void clear();

    PlayerList load_players(assets::Manager& assets, const GameField& field, ObjectTypeFactory::res_type& types);

    EventList get_events(sf::Keyboard::Key);

    void draw_markers(Window& window);

  private:
    using KeyActions = std::unordered_map<sf::Keyboard::Key, std::string>;

    struct PlayerEntry {
        PlayerEntry(const std::shared_ptr<GameObject>& player, const PlayerMarkerProps& marker_props,
                    assets::Fonts& fonts, KeyActions&& key_map);

        std::shared_ptr<GameObject> player;
        KeyActions key_map;
        PlayerMarker marker;
        sf::Vector2f init_pos;
    };

    bool add_player(const std::shared_ptr<GameObject>&, const PlayerMarkerProps& marker_props, assets::Fonts& fonts,
                    const PlayerOptions&);

    PlayerLoader loader_;

    std::map<size_t, PlayerEntry> players_;

    std::string score_key_ = "score";
};

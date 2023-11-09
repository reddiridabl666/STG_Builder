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

    void erase_player(size_t id);
    void clear();

    PlayerList load_players(assets::Manager& assets, const GameField& field, ObjectTypeFactory::res_type& types);

    EventList get_events(sf::Keyboard::Key);

  private:
    void add_player(const std::shared_ptr<GameObject>&, const PlayerOptions&);

    using KeyActions = std::unordered_map<sf::Keyboard::Key, std::string>;

    PlayerLoader loader_;

    std::vector<std::shared_ptr<GameObject>> players_;
    std::vector<KeyActions> key_map_;
};

#include "PlayerManager.hpp"

#include "TextFactory.hpp"

void PlayerManager::add_player(const std::shared_ptr<GameObject>& player, const PlayerOptions& opts) {
    players_.push_back(player);

    KeyActions key_map;
    for (auto& [action, keys] : opts.keys.other) {
        for (auto key : keys) {
            key_map[key] = action;
        }
    }
    key_map_.push_back(key_map);
}

void PlayerManager::erase_player(size_t id) {
    players_.erase(players_.begin() + id);
    key_map_.erase(key_map_.begin() + id);
    markers_.erase(markers_.begin() + id);
}

void PlayerManager::clear() {
    players_.clear();
    key_map_.clear();
}

PlayerList PlayerManager::load_players(assets::Manager& assets, const GameField& field,
                                       ObjectTypeFactory::res_type& types) {
    auto players = loader_.load_players(assets, field, types);

    PlayerList res;
    res.reserve(players.size());

    auto marker_props = loader_.marker_props();

    for (auto& [player, opts] : players) {
        add_player(player, opts);
        markers_.push_back(PlayerMarker{
            engine::TextFactory::create(marker_props.text, assets.fonts()),
            player,
            marker_props.offset,
        });
        res.push_back(std::move(player));
    }

    return res;
}

PlayerManager::EventList PlayerManager::get_events(sf::Keyboard::Key key) {
    EventList res;

    for (size_t i = 0; i < key_map_.size(); ++i) {
        auto event = key_map_[i].find(key);
        if (event == key_map_[i].end()) {
            continue;
        }
        res.push_back(std::pair(event->second, players_[i]));
    }

    return res;
}

void PlayerManager::draw_markers(Window& window) {
    for (auto& marker : markers_) {
        marker.draw(window);
    }
}

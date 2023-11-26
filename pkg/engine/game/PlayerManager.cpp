#include "PlayerManager.hpp"

#include <algorithm>
#include <ranges>

#include "TextFactory.hpp"

bool PlayerManager::add_player(const std::shared_ptr<GameObject>& player, const PlayerMarkerProps& marker_props,
                               assets::Fonts& fonts, const PlayerOptions& opts) {
    size_t id = player->props().get(kPlayerNum);

    auto it = players_.find(id);
    if (it != players_.end()) {
        return false;
    }

    KeyActions key_map;
    for (auto& [action, keys] : opts.keys.other) {
        for (auto key : keys) {
            key_map[key] = action;
        }
    }

    players_.emplace(id, PlayerEntry(player, marker_props, fonts, std::move(key_map)));
    return true;
}

long PlayerManager::get_score() const {
    auto max_score_player = std::max_element(players_.begin(), players_.end(), [this](const auto& l, const auto& r) {
        return l.second.player->props().get(score_key_) < r.second.player->props().get(score_key_);
    });

    if (max_score_player == players_.end()) {
        return 0;
    }

    return max_score_player->second.player->props().get(score_key_);
}

void PlayerManager::erase_player(size_t id) {
    players_.erase(id);
}

void PlayerManager::clear() {
    players_.clear();
}

PlayerList PlayerManager::load_players(assets::Manager& assets, const GameField& field,
                                       ObjectTypeFactory::res_type& types) {
    auto players = loader_.load_players(assets, field, types);

    PlayerList res;
    res.reserve(players.size());

    auto marker_props = loader_.marker_props();

    for (auto& [player, opts] : players) {
        if (add_player(player, marker_props, assets.fonts(), opts)) {
            res.push_back(std::move(player));
        }
    }

    return res;
}

PlayerManager::EventList PlayerManager::get_events(sf::Keyboard::Key key) {
    EventList res;

    for (const auto& [_, entry] : players_) {
        auto event = entry.key_map.find(key);
        if (event == entry.key_map.end()) {
            continue;
        }
        res.push_back(std::pair(event->second, entry.player));
    }

    return res;
}

void PlayerManager::draw_markers(Window& window) {
    for (auto& [_, player] : players_) {
        player.marker.draw(window);
    }
}

PlayerManager::PlayerEntry::PlayerEntry(const std::shared_ptr<GameObject>& obj, const PlayerMarkerProps& marker_props,
                                        assets::Fonts& fonts, KeyActions&& key_map)
    : player(obj),
      key_map(std::move(key_map)),
      marker(engine::TextFactory::create(marker_props.text, fonts), player, marker_props.offset) {}

#include "GameFactory.hpp"

#include <stdexcept>

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace engine {
namespace {
PlayerOptions generate_options(const nl::json& player_json, int player_num) {
    if (!player_json.contains("opts")) {
        return PlayerOptions{.num = player_num};
    }

    auto res = player_json.at("opts").template get<PlayerOptions>();
    res.num = player_num;
    return res;
}

PlayerList::value_type generate_player(const nl::json& player_json, int player_num = 1) {
    auto player_type = ObjectTypeFactory::generate("player", player_json);
    if (!player_type) {
        throw std::runtime_error(player_type.error().message());
    }

    return std::pair(*player_type, generate_options(player_json, player_num));
}
}  // namespace

PlayerList GameFactory::generate_players(const nl::json& game) {
    PlayerList res;

    if (game.contains("player")) {
        res.push_back(generate_player(game.at("player")));
        return res;
    }

    auto players_json = game.at("players");
    if (!players_json.is_array()) {
        throw std::runtime_error("'players' should be an array");
    }

    res.reserve(players_json.size());

    size_t idx = 1;
    for (const auto& [_, value] : players_json.items()) {
        res.push_back(generate_player(value, idx));
        ++idx;
    }

    return res;
}
}  // namespace engine

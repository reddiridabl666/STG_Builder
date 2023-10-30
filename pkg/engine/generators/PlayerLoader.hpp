#pragma once

#include "GameField.hpp"
#include "ObjectOptionsFactory.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"

class PlayerLoader {
  public:
    PlayerLoader(const nl::json& players) : players_(players) {}

    ErrorOr<PlayerList> load_players(assets::Manager& textures, const GameField& field,
                                     ObjectTypeFactory::res_type& types);

  private:
    nl::json players_;
};

inline ErrorOr<PlayerList> PlayerLoader::load_players(assets::Manager& assets, const GameField& field,
                                                      ObjectTypeFactory::res_type& types) {
    ObjectOptionsFactory opts_factory_(field);
    PlayerList res;

    res.reserve(players_.size());

    for (auto& [_, player_json] : players_.items()) {
        auto opts = opts_factory_.generate(player_json);
        if (!opts) {
            return tl::unexpected(opts.error());
        }

        auto player = types.at(opts->type).create_player(*opts, assets, player_json.at("opts").get<PlayerOptions>());
        res.push_back(std::move(*player));
    }

    return res;
}

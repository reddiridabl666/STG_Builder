#pragma once

#include "GameField.hpp"
#include "ObjectOptionsFactory.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"

class PlayerLoader {
  public:
    PlayerLoader(const nl::json& players) : players_(players) {}

    PlayerList load_players(assets::Manager& assets, const GameField& field, ObjectTypeFactory::res_type& types);

    PlayerList::value_type load_player(size_t idx, assets::Manager& assets, ObjectType& obj_type,
                                       const ObjectOptionsFactory& opts_factory);

  private:
    nl::json players_;
};

inline PlayerList PlayerLoader::load_players(assets::Manager& assets, const GameField& field,
                                             ObjectTypeFactory::res_type& types) {
    ObjectOptionsFactory opts_factory(field);
    PlayerList res;

    res.reserve(players_.size());

    for (size_t idx = 0; idx < players_.size(); ++idx) {
        auto obj_type_name = players_[idx].at("type").get<std::string>();

        auto player = load_player(idx, assets, types.at(obj_type_name), opts_factory);

        res.push_back(std::move(player));
    }

    return res;
}

inline PlayerList::value_type PlayerLoader::load_player(size_t idx, assets::Manager& assets, ObjectType& obj_type,
                                                        const ObjectOptionsFactory& opts_factory) {
    auto opts = opts_factory.generate(players_[idx]);

    auto player_opts = players_[idx].at("opts").get<PlayerOptions>();
    player_opts.num = idx;

    return obj_type.create_player(opts, assets, player_opts);
}

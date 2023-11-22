#pragma once

#include "GameField.hpp"
#include "ObjectOptionsFactory.hpp"
#include "ObjectTypeFactory.hpp"
#include "Player.hpp"
#include "Text.hpp"

class PlayerLoader {
  private:
    using PlayerWithOptions = std::pair<PlayerList::value_type, PlayerOptions>;

  public:
    PlayerLoader(const nl::json& players, PlayerMarkerProps&& props)
        : players_(players), marker_props_(std::move(props)) {}

    std::vector<PlayerWithOptions> load_players(assets::Manager& assets, const GameField& field,
                                                ObjectTypeFactory::res_type& types);

    PlayerWithOptions load_player(size_t idx, assets::Manager& assets, ObjectType& obj_type,
                                  const ObjectOptionsFactory& opts_factory);

    const auto& marker_props() const {
        return marker_props_;
    }

  private:
    nl::json players_;
    PlayerMarkerProps marker_props_;
};

inline std::vector<PlayerLoader::PlayerWithOptions> PlayerLoader::load_players(assets::Manager& assets,
                                                                               const GameField& field,
                                                                               ObjectTypeFactory::res_type& types) {
    ObjectOptionsFactory opts_factory(field);
    std::vector<PlayerWithOptions> res;

    res.reserve(players_.size());

    for (size_t idx = 0; idx < players_.size(); ++idx) {
        auto obj_type_name = players_[idx].at("type").get<std::string>();

        auto player = load_player(idx, assets, types.at(obj_type_name), opts_factory);

        res.push_back(std::move(player));
    }

    return res;
}

inline PlayerLoader::PlayerWithOptions PlayerLoader::load_player(size_t idx, assets::Manager& assets,
                                                                 ObjectType& obj_type,
                                                                 const ObjectOptionsFactory& opts_factory) {
    auto opts = opts_factory.generate(players_[idx]);

    auto player_opts = players_[idx].at("opts").get<PlayerOptions>();
    player_opts.num = idx;

    return std::pair(obj_type.create_player(opts, assets, player_opts), player_opts);
}

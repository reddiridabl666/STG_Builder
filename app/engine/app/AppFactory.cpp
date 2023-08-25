#include "AppFactory.hpp"

#include <stdexcept>

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace {
PlayerList generate_players(const nl::json& game);
}  // namespace

App AppFactory::generate(const nl::json& game, const nl::json& entities, const std::string& base_dir) const {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    // TODO: do something about the side menu, game field and the player
    // TODO: should directories be hardcoded?
    AssetManager<sf::Texture> textures(AssetLoader{base_dir + "/assets/images"}, AssetStorage<sf::Texture>{});
    AssetManager<sf::SoundBuffer> sounds(AssetLoader{base_dir + "/assets/sounds"},
                                         AssetStorage<sf::SoundBuffer>{});

    WindowInfo window_info{
        .name = game.at("name").template get<std::string>(),
        .size = game.at("size").template get<sf::Vector2i>(),
    };

    LevelManager levels(game.at("levels").get<int>(), LevelLoader{base_dir + "/level"});

    return App(window_info, generate_players(game), std::move(textures), std::move(sounds),
               std::move(types.value()), std::move(levels));
}

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

PlayerList generate_players(const nl::json& game) {
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
}  // namespace

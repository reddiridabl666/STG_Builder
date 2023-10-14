#pragma once

#include "Game.hpp"

namespace engine {
class GameFactory {
  public:
    template <typename GameType>
    static GameType generate(Window& window, const nl::json& game, const nl::json& entities,
                             const std::string& base_dir);

    template <typename GameType>
    static std::unique_ptr<GameType> generate_unique(Window& window, const nl::json& game,
                                                     const nl::json& entities, const std::string& base_dir);

  private:
    static PlayerList generate_players(const nl::json& game);
    static ObjectOptionsFactory opts_factory_;
};

template <typename GameType = Game<>>
GameType GameFactory::generate(Window& window, const nl::json& game, const nl::json& entities,
                               const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    // TODO: do something about the side menu
    // TODO: should directories be hardcoded?
    const auto img_path = base_dir + "/assets/images";
    assets::Textures textures(img_path);
    assets::Sounds sounds(base_dir + "/assets/sounds");

    LevelLoader level_loader(base_dir + "/level",
                             json::get<sf::FloatRect>(game, "field_size", GameField::kDefaultRatio),
                             img_path);

    LevelManager levels(json::get<int>(game, "levels"), std::move(level_loader));

    return GameType{
        window,
        PlayerLoader(game.at("players")),
        std::move(textures),
        std::move(sounds),
        std::move(types.value()),
        std::move(levels),
        json::get<int>(game, "fps", 60),
    };
}

template <typename GameType>
std::unique_ptr<GameType> GameFactory::generate_unique(Window& window, const nl::json& game,
                                                       const nl::json& entities,
                                                       const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    const auto img_path = base_dir + "/assets/images";
    assets::Textures textures(img_path);
    assets::Sounds sounds(base_dir + "/assets/sounds");

    LevelLoader level_loader(base_dir + "/level",
                             json::get<sf::FloatRect>(game, "field_size", GameField::kDefaultRatio),
                             img_path);
    LevelManager levels(json::get<int>(game, "levels"), std::move(level_loader));

    auto fps = json::get<int>(game, "fps", 60);

    return std::make_unique<GameType>(window, PlayerLoader(game.at("players")), std::move(textures),
                                      std::move(sounds), std::move(types.value()), std::move(levels), fps);
}
}  // namespace engine

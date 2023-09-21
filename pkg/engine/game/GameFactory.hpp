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
};

template <typename GameType>
GameType GameFactory::generate(Window& window, const nl::json& game, const nl::json& entities,
                               const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    // TODO: do something about the side menu, game field and the player
    // TODO: should directories be hardcoded?
    AssetManager<sf::Texture> textures(AssetLoader{base_dir + "/assets/images"}, AssetStorage<sf::Texture>{});
    AssetManager<sf::SoundBuffer> sounds(AssetLoader{base_dir + "/assets/sounds"},
                                         AssetStorage<sf::SoundBuffer>{});

    LevelLoader level_loader(base_dir + "/level", game.at("field_size").get<sf::FloatRect>());
    LevelManager levels(game.at("levels").get<int>(), std::move(level_loader));

    auto fps = json::get<int>(game, "fps", 60);

    return GameType(window, generate_players(game), std::move(textures), std::move(sounds),
                    std::move(types.value()), std::move(levels), fps);
}

template <typename GameType>
std::unique_ptr<GameType> GameFactory::generate_unique(Window& window, const nl::json& game,
                                                       const nl::json& entities,
                                                       const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    AssetManager<sf::Texture> textures(AssetLoader{base_dir + "/assets/images"}, AssetStorage<sf::Texture>{});
    AssetManager<sf::SoundBuffer> sounds(AssetLoader{base_dir + "/assets/sounds"},
                                         AssetStorage<sf::SoundBuffer>{});

    LevelLoader level_loader(base_dir + "/level", game.at("field_size").get<sf::FloatRect>());
    LevelManager levels(game.at("levels").get<int>(), std::move(level_loader));

    auto fps = json::get<int>(game, "fps", 60);

    return std::make_unique<GameType>(window, generate_players(game), std::move(textures), std::move(sounds),
                                      std::move(types.value()), std::move(levels), fps);
}
}  // namespace engine

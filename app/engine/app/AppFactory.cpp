#include "AppFactory.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

ErrorOr<App> AppFactory::generate(Window& window, const nl::json& game, const nl::json& entities,
                                  const std::string& level_prefix) const {
    auto types = ObjectTypeFactory{}.generate(entities);
    if (!types) {
        return tl::unexpected(types.error());
    }

    // TODO: do something about the side menu, game field and the player

    AssetManager<sf::Texture> textures(AssetLoader{}, AssetStorage<sf::Texture>{});
    AssetManager<sf::SoundBuffer> sounds(AssetLoader{}, AssetStorage<sf::SoundBuffer>{});

    try {
        LevelManager levels(game.at("levels").get<int>(), LevelLoader{});
        return App(window, std::move(textures), std::move(sounds), std::move(types.value()),
                   std::move(levels));
    } catch (nl::json::exception& e) {
        return unexpected_error<InternalError>(e.what());
        // TODO: would be better to check for different json exceptions
    }
}

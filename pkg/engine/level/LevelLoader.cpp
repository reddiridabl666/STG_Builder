#include "LevelLoader.hpp"

#include <fmt/core.h>

#include <fstream>
#include <tl/expected.hpp>

#include "AssetManager.hpp"
#include "Json.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace engine {
ErrorOr<Level> LevelLoader::load_level(Window& window, AssetManager<sf::Texture>& textures,
                                       size_t number) const {
    auto level_name = fmt::format("{}_{}.json", prefix_, number);

#ifdef DEBUG
    LOG(fmt::format("Reading level file: {}", level_name));
#endif

    auto j = json::read(level_name);
    if (!j) {
        return tl::unexpected(j.error());
    }

    try {
#ifdef DEBUG
        LOG("Creating level struct");
#endif
        auto field = load_field(window, textures, j->at("bg"));
        if (!field) {
            return tl::unexpected(field.error());
        }

        ObjectOptionsFactory factory(*field);
        auto opts = factory.generate(j->at("entities").get<std::vector<nl::json>>());
        if (!opts) {
            return tl::unexpected(opts.error());
        }

        return Level{
            j->at("name").get<std::string>(),
            std::move(*field),
            std::move(*opts),
        };
    } catch (std::exception& e) {
        return Error::New(e.what());
    }
}

ErrorOr<GameField> LevelLoader::load_field(Window& window, AssetManager<sf::Texture>& textures,
                                           nl::json json) const {
#ifdef DEBUG
    LOG("Loading game field");
#endif
    auto texture = textures.get(json.at("image").template get<std::string>());
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto sprite = std::make_unique<SpriteObject>(std::move(*texture));

    return GameField{
        std::move(sprite),
        window,
        field_size_,
        json.at("speed").template get<int>(),
    };
}
}  // namespace engine

#include "LevelLoader.hpp"

#include <fmt/core.h>

#include <fstream>
#include <tl/expected.hpp>

#include "Json.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace engine {
ErrorOr<Level> LevelLoader::load_level(Window& window, size_t number) const {
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
        auto field = load_field(window, j->at("bg"));
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

ErrorOr<GameField> LevelLoader::load_field(Window& window, const nl::json& field_json) const {
#ifdef DEBUG
    LOG("Loading game field");
#endif
    auto texture = assets_.load<sf::Texture>(field_json.at("image").template get<std::string>());
    if (!texture) {
        return tl::unexpected(texture.error());
    }

#ifdef DEBUG
    LOG("Loaded game field background");
#endif

    auto sprite = std::make_unique<SpriteObject>(std::move(*texture));

    return GameField{
        std::move(sprite),
        window,
        field_size_,
        field_json.at("speed").template get<int>(),
    };
}
}  // namespace engine

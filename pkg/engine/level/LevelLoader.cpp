#include "LevelLoader.hpp"

#include <fmt/core.h>

#include <fstream>
#include <tl/expected.hpp>

#include "Json.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace engine {
Level LevelLoader::load_level(Window& window, size_t number) const {
    auto level_name = fmt::format("{}_{}.json", prefix_, number);

#ifdef DEBUG
    LOG(fmt::format("Reading level file: {}", level_name));
#endif

    auto j = json::read(level_name);
    if (!j) {
        throw j.error();
    }

#ifdef DEBUG
    LOG("Creating level struct");
#endif
    auto field = load_field(window, j->at("bg"));

    ObjectOptionsFactory factory(field);
    auto opts = factory.generate(j->at("entities").get<std::vector<nl::json>>());

    return Level{
        j->at("name").get<std::string>(),
        std::move(field),
        std::move(opts),
    };
}

GameField LevelLoader::load_field(Window& window, const nl::json& field_json) const {
#ifdef DEBUG
    LOG("Loading game field");
#endif
    auto texture = assets_.load<sf::Texture>(field_json.at("image").template get<std::string>());
    if (!texture) {
        throw std::runtime_error("failure loading game field background");
    }

#ifdef DEBUG
    LOG("Loaded game field background");
#endif

    auto sprite = std::make_unique<SpriteObject>(std::move(*texture));

    return GameField{
        std::move(sprite),
        window,
        field_size_,
        field_json.value("speed", 100),
    };
}
}  // namespace engine

#include "DisplayableFactory.hpp"

#include "DummyDisplayable.hpp"
#include "ShapeObject.hpp"
#include "SpriteObject.hpp"

static constexpr float kDefaultSize = 50;

std::unique_ptr<Displayable> DisplayableFactory::create(const nl::json& json, assets::Textures& assets) {
    if (json == "none") {
        return std::make_unique<DummyDisplayable>();
    }

    if (!json.is_object()) {
        return nullptr;
    }

    // clang-format off
    if (json.contains("image")) {
        auto res = std::make_unique<SpriteObject>(
            assets.get_or(json.value("image", ""), assets::kFallbackImage)
        );
        if (json.contains("size")) {
            res->set_size(json.value("size", sf::Vector2f{kDefaultSize, kDefaultSize}));
        }
        return res;
    }

    auto type = json.value("type", "rect");
    if (type == "rect") {
        return std::make_unique<RectObject>(
            json.value("size", sf::Vector2f{kDefaultSize, kDefaultSize}),
            json.value("fill_color", sf::Color::Black),
            json.value("outline_color", sf::Color::White),
            json.value("outline", 1zu)
        );
    }

    if (type == "circle") {
        return std::make_unique<CircleObject>(
            json.value("radius", kDefaultSize),
            json.value("fill_color", sf::Color::Black),
            json.value("outline_color", sf::Color::White),
            json.value("outline", 1zu)
        );
    }
    // clang-format on

    return nullptr;
}

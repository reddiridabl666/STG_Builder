#include "PatternFactory.hpp"

#include "Math.hpp"
#include "patterns/ArcPosition.hpp"
#include "patterns/DelegatedPattern.hpp"
#include "patterns/FollowingParentMovement.hpp"
#include "patterns/FromCenterMovement.hpp"
#include "patterns/LinePosition.hpp"
#include "patterns/LinearMovement.hpp"
#include "patterns/ObjectCount.hpp"
#include "patterns/RectanglePosition.hpp"
#include "patterns/SpreadPosition.hpp"
#include "patterns/TargetedMovement.hpp"

struct PatternMovementFactory {
    static std::unique_ptr<Pattern::MovementSetter> create(const nl::json& json) {
        if (!json.is_object()) {
            return nullptr;
        }

        std::string type = json.value("type", "");

        if (type == "linear") {
            return std::make_unique<LinearMovement>(json.value("velocity", sf::Vector2f{0, -1}));
        }

        if (type == "from_center") {
            return std::make_unique<FromCenterMovement>();
        }

        if (type == "follow_parent") {
            return std::make_unique<FollowingParentMovement>();
        }

        if (type == "targeted") {
            return std::make_unique<TargetedMovement>(json.value("tag", GameObjectTag::Player));
        }

        if (type == "random") {
            return nullptr;
        }

        return nullptr;
    }
};

struct PatternPositionFactory {
    static std::unique_ptr<Pattern::PositionSetter> create(const nl::json& json) {
        std::string type = json.value("type", "");

        // clang-format off
        if (type == "line") {
            return std::make_unique<LinePosition>(
                json.value("parent_offset", sf::Vector2f{}),
                json.value("sibling_offset", sf::Vector2f{})
            );
        }

        if (type == "circle") {
            return std::make_unique<ArcPosition>(
                json.value("offset", sf::Vector2f{}),
                json.value("radius", 0.f),
                math::to_radians(json.value("start_angle", 0.f)),
                math::to_radians(json.value("angle", 360.f))
            );
        }

        // if (type == "rect") {
        //     return std::make_unique<RectanglePosition>(
        //     );
        // }

        if (type == "spread") {
            return std::make_unique<SpreadPosition>(
                json.value("offset", sf::Vector2f{}),
                json.value("space", sf::Vector2f{})
            );
        }
        // clang-format on

        return nullptr;
    }
};

struct PatternCountFactory {
    static std::unique_ptr<Pattern::ObjectCount> create(const nl::json& json) {
        if (json.is_number()) {
            return std::make_unique<ConstCount>(json.get<float>());
        }

        if (!json.is_string()) {
            return nullptr;
        }

        auto [left, right] = rng::parse(json.get<std::string>());
        return std::make_unique<RandomCount>(left, right);
    }
};

std::unique_ptr<Pattern> PatternFactory::create(const nl::json& json) {
    auto pos = PatternPositionFactory::create(json.at("pos"));
    auto move = PatternMovementFactory::create(json.at("move"));
    auto count = PatternCountFactory::create(json.at("count"));

    return std::make_unique<DelegatedPattern>(json.value("bullet", ""), std::move(count), std::move(move),
                                              std::move(pos));
}

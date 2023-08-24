#include "FuncBuilder.hpp"

#include "Life.hpp"
#include "Movement.hpp"

template <>
movement::Func FuncBuilder::generate(const MoveInfo& info) {
    // clang-format off
    if (info.name == "linear") {
        return movement::linear(
            info.args.at("x").get<float>(),
            info.args.at("y").get<float>()
        );
    }

    if (info.name == "circular") {
        return movement::circular(
            info.args.at("center").get<sf::Vector2f>(),
            info.args.at("speed").get<float>()
        );
    }
    // clang-format on

    throw std::runtime_error("Unsupported move func type");
}

template <>
life::update FuncBuilder::generate(const LifeInfo& info) {
    if (info.name == "bounds") {
        return life::in_bounds(info.args.at("margin"));
    }

    if (info.name == "timed") {
        return life::timed(info.args.at("seconds"));
    }

    throw std::runtime_error("Unsupported life func type");
}

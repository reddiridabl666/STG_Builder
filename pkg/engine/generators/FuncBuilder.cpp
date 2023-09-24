#include "FuncBuilder.hpp"

#include "Life.hpp"
#include "Movement.hpp"

template <>
movement::Func FuncBuilder::generate(const FuncInfo& info) {
    // clang-format off
    if (info.type == "linear") {
        return movement::linear(
            info.args.at("x").get<float>(),
            info.args.at("y").get<float>()
        );
    }

    if (info.type == "circular") {
        return movement::circular(
            info.args.at("center").get<sf::Vector2f>(),
            info.args.at("speed").get<float>()
        );
    }
    // clang-format on

    throw std::runtime_error("Unsupported move func type");
}

template <>
alive::update FuncBuilder::generate(const FuncInfo& info) {
    if (info.type == "bounds") {
        return alive::in_bounds(info.args.at("margin").get<float>());
    }

    if (info.type == "timed") {
        return alive::timed(info.args.at("seconds").get<float>());
    }

    throw std::runtime_error("Unsupported alive func type");
}

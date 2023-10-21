#include "FuncBuilder.hpp"

#include "Life.hpp"

template <>
std::unique_ptr<movement::Rule> FuncBuilder::generate(const FuncInfo& info) {
    // clang-format off
    if (info.type == "" || info.type == "none") {
        return movement::no_op();
    }

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

    if (info.type == "user_control") {
        return movement::user_control();
    }
    // clang-format on

    throw std::runtime_error("Unsupported move func type");
}

template <>
alive::update FuncBuilder::generate(const FuncInfo& info) {
    if (info.type == "" || info.type == "default") {
        return alive::default_func;
    }

    if (info.type == "bounds") {
        return alive::in_bounds(info.args.at("margin").get<float>());
    }

    if (info.type == "always") {
        return alive::always;
    }

    if (info.type == "timed") {
        return alive::timed(info.args.at("seconds").get<float>());
    }

    throw std::runtime_error("Unsupported alive func type");
}

std::unique_ptr<movement::Rule> FuncBuilder::generate(const movement::MultiInfo& info) {
    std::vector<std::unique_ptr<movement::Rule>> funcs;
    std::vector<float> times;

    funcs.reserve(info.rules.size());
    times.reserve(info.rules.size());

    for (auto& el : info.rules) {
        funcs.push_back(generate<std::unique_ptr<movement::Rule>>(el));
        times.push_back(el.time);
    }

    return std::make_unique<movement::Multi>(std::move(funcs), std::move(times), info.repeat);
}

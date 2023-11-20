#include "FuncBuilder.hpp"

#include "Life.hpp"

namespace {
std::vector<alive::update> get_multiple(const nl::json& json);
alive::update alive_condition(const nl::json& json);
}  // namespace

template <>
std::unique_ptr<movement::Rule> FuncBuilder::generate(const FuncInfo& info) {
    // clang-format off
    if (info.type == "" || info.type == "none") {
        return movement::no_op();
    }

    if (info.type == "linear") {
        return movement::linear(
            info.args.get<sf::Vector2f>()
        );
    }

    if (info.type == "circular") {
        return movement::circular(
            info.args.at("radius").get<sf::Vector2f>(),
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

    if (info.type == "all") {
        return alive::all(get_multiple(info.args));
    }

    if (info.type == "any") {
        return alive::any(get_multiple(info.args));
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

    if (info.type == "condition") {
        return alive_condition(info.args);
    }

    return nullptr;
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

namespace {
std::vector<alive::update> get_multiple(const nl::json& json) {
    std::vector<alive::update> funcs;
    funcs.reserve(json.size());

    for (auto& [_, value] : json.items()) {
        auto res = FuncBuilder::generate<alive::update>(value.get<FuncInfo>());
        if (res) {
            funcs.push_back(res);
        }
    }

    return funcs;
}

alive::update alive_condition(const nl::json& json) {
    static const std::unordered_map<std::string, std::function<bool(float, float)>> op_map = {
        {"<", std::less<float>{}},           {">", std::greater<float>{}},   {"<=", std::less_equal<float>{}},
        {">=", std::greater_equal<float>{}}, {"==", std::equal_to<float>{}}, {"!=", std::not_equal_to<float>{}},
    };

    std::string property = json.value("property", "health");
    std::string op = json.value("op", ">");
    float value = json.value("than", 0);

    if (!op_map.contains(op)) {
        op = ">";
    }

    return alive::property_condition(property, op_map.at(op), value);
}
}  // namespace

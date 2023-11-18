#include "ActionFactory.hpp"

#include "ActionTypes.hpp"
#include "Decorators.hpp"

namespace action {

namespace {
std::unique_ptr<MutableGetter> create_mutable_getter(std::string&& property) {
    if (property == "speed") {
        return std::make_unique<SpeedGetter>();
    }

    return std::make_unique<ObjectPropertyGetter>(std::move(property));
}

std::unique_ptr<Getter> create_undecorated_getter(const nl::json& json) {
    if (json.is_object()) {
        return create_undecorated_getter(json.at("property"));
    }

    if (json.is_number()) {
        return std::make_unique<ConstGetter>(json.get<float>());
    }

    if (!json.is_string()) {
        return nullptr;
    }

    auto property = json.get<std::string>();
    if (property.starts_with("random(")) {
        auto [left, right] = rng::parse(property);
        return std::make_unique<RandomGetter>(left, right);
    }

    return create_mutable_getter(std::move(property));
}

std::unique_ptr<Getter> create_getter_decorator(std::unique_ptr<Getter>& original, const std::string& key,
                                                const nl::json& value) {
    if (key == "mul") {
        return std::make_unique<MultiplyGetterDecorator>(std::move(original), value.get<float>());
    }

    return nullptr;
}

std::unique_ptr<Getter> create_getter(const nl::json& json) {
    std::unique_ptr<Getter> res = create_undecorated_getter(json);
    if (!res) {
        return res;
    }

    auto decorators = json.find("with");
    if (decorators == json.end()) {
        return res;
    }

    for (auto& [key, value] : decorators->items()) {
        auto decorator = create_getter_decorator(res, key, value);
        if (!decorator) {
            continue;
        }
        res = std::move(decorator);
    }

    return res;
}

std::unique_ptr<BinaryAction> create_binary_action(const std::string& type, std::unique_ptr<MutableGetter>& property,
                                                   const nl::json& value) {
    auto getter = create_getter(value);
    if (!getter) {
        return nullptr;
    }

    if (type == "set") {
        return std::make_unique<PropertySetter>(std::move(property), std::move(getter));
    }

    if (type == "add") {
        return std::make_unique<PropertyAdder>(std::move(property), std::move(getter));
    }

    if (type == "sub") {
        return std::make_unique<PropertySubber>(std::move(property), std::move(getter));
    }

    if (type == "mul") {
        return std::make_unique<PropertyMultiplier>(std::move(property), std::move(getter));
    }

    if (type == "div") {
        return std::make_unique<PropertyDivisor>(std::move(property), std::move(getter));
    }

    return nullptr;
}

std::unique_ptr<Action> create_unary_action(const std::string& type, const std::string& property) {
    if (type == "die") {
        return std::make_unique<Killer>();
    }

    if (type == "toggle_visibility") {
        return std::make_unique<VisibilityToggler>();
    }

    if (type == "shoot") {
        return std::make_unique<Shooter>(property);
    }

    return nullptr;
}

std::unique_ptr<Action> create_unary_property_action(const std::string& type,
                                                     std::unique_ptr<MutableGetter>& property) {
    if (type == "inc") {
        return std::make_unique<Incrementor>(std::move(property));
    }

    if (type == "dec") {
        return std::make_unique<Decrementor>(std::move(property));
    }

    if (type == "reset") {
        return std::make_unique<Resetter>(std::move(property));
    }

    return nullptr;
}

template <typename ActionType>
std::unique_ptr<ActionType> create_undecorated_action(const nl::json& json);

template <>
std::unique_ptr<BinaryAction> create_undecorated_action<BinaryAction>(const nl::json& json) {
    std::string type = json.value("action", "");
    std::string property_value = json.value("property", "");

    if (type == "") {
        return nullptr;
    }

    if (property_value == "" || type == "shoot") {
        return BinaryAdapter::wrap(create_unary_action(type, property_value));
    }

    auto property = create_mutable_getter(std::move(property_value));
    if (!property) {
        return nullptr;
    }

    auto value = json.find("value");
    if (value == json.end()) {
        return BinaryAdapter::wrap(create_unary_property_action(type, property));
    }

    return create_binary_action(type, property, *value);
}

template <>
std::unique_ptr<Action> create_undecorated_action<Action>(const nl::json& json) {
    std::string type = json.value("action", "");
    std::string property_value = json.value("property", "");

    if (type == "") {
        return nullptr;
    }

    if (property_value == "" || type == "shoot") {
        return create_unary_action(type, property_value);
    }

    auto property = create_mutable_getter(std::move(property_value));
    if (!property) {
        return nullptr;
    }

    auto value = json.find("value");
    if (value == json.end()) {
        return create_unary_property_action(type, property);
    }

    return create_binary_action(type, property, *value);
}

template <typename ActionType>
std::unique_ptr<ActionType> create_action_decorator(std::unique_ptr<ActionType>& original, const std::string& key,
                                                    const nl::json& value) {
    if (key == "timeout") {
        return std::make_unique<TimeoutDecorator<ActionType>>(std::move(original), value.get<float>());
    }

    if (key == "delay") {
        return std::make_unique<DelayDecorator<ActionType>>(std::move(original), value.get<float>());
    }

    return nullptr;
}

template <typename ActionType>
void decorate_action(std::unique_ptr<ActionType>& original, const nl::json& decorators) {
    for (auto& [key, value] : decorators.items()) {
        auto decorator = create_action_decorator<ActionType>(original, key, value);
        if (!decorator) {
            continue;
        }
        original = std::move(decorator);
    }
}

template <typename ActionType>
std::unique_ptr<ActionType> create_single_action(const nl::json& json) {
    std::unique_ptr<ActionType> res = create_undecorated_action<ActionType>(json);
    if (!res) {
        return res;
    }

    auto decorators = json.find("with");
    if (decorators == json.end()) {
        return res;
    }

    decorate_action(res, *decorators);

    return res;
}

template <typename ActionType>
std::unique_ptr<ActionType> create_multi_action(const nl::json& json) {
    fmt::println("actions: {}", json.dump(4));

    std::vector<std::unique_ptr<ActionType>> actions;
    actions.reserve(json.size());

    for (const auto& [_, value] : json.items()) {
        auto action = create_single_action<ActionType>(value);
        if (action) {
            actions.push_back(std::move(action));
        }
    }

    return std::make_unique<MultiAction<ActionType>>(std::move(actions));
}
}  // namespace

template <typename ActionType>
std::unique_ptr<ActionType> Factory::create(const nl::json& json) {
    fmt::println("actions: {}", json.dump(4));
    if (json.is_array()) {
        return create_multi_action<ActionType>(json);
    }

    if (!json.is_object()) {
        return nullptr;
    }

    auto actions = json.find("actions");
    if (actions != json.end()) {
        auto res = create_multi_action<ActionType>(*actions);
        auto decorators = json.find("with");
        if (decorators == json.end()) {
            return res;
        }

        decorate_action(res, *decorators);
        return res;
    }

    return create_single_action<ActionType>(json);
}

template std::unique_ptr<Action> Factory::create<Action>(const nl::json& json);
template std::unique_ptr<BinaryAction> Factory::create<BinaryAction>(const nl::json& json);
}  // namespace action

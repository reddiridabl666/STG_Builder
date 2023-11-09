#include "ActionFactory.hpp"

#include "ActionTypes.hpp"
#include "Decorators.hpp"

namespace action {

namespace {
std::unique_ptr<MutableGetter> create_mutable_getter(const nl::json& json) {
    if (json.is_string()) {
        auto property = json.get<std::string>();
        if (property == "speed") {
            return std::make_unique<SpeedGetter>();
        }
        return std::make_unique<ObjectPropertyGetter>(std::move(property));
    }

    return nullptr;
}

std::unique_ptr<Getter> create_undecorated_getter(const nl::json& json) {
    if (json.is_object()) {
        return create_undecorated_getter(json.at("property"));
    }

    if (json.is_number()) {
        return std::make_unique<ConstGetter>(json.get<float>());
    }

    return create_mutable_getter(json);
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

std::unique_ptr<BinaryAction> create_undecorated_action(const nl::json& json) {
    std::string type = json.value("action", "");
    std::string property_value = json.value("property", "");

    if (property_value == "") {
        return nullptr;
    }

    if (type == "") {
        return nullptr;
    }

    auto property = create_mutable_getter(property_value);

    if (type == "inc") {
        return std::make_unique<Incrementor>(std::move(property));
    }

    if (type == "dec") {
        return std::make_unique<Decrementor>(std::move(property));
    }

    if (type == "reset") {
        return std::make_unique<Resetter>(std::move(property));
    }

    auto value = json.find("value");
    if (value == json.end()) {
        return nullptr;
    }

    auto getter = create_getter(*value);
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

std::unique_ptr<BinaryAction> create_action_decorator(std::unique_ptr<BinaryAction>& original, const std::string& key,
                                                      const nl::json& value) {
    if (key == "timeout") {
        return std::make_unique<TimeoutDecorator>(std::move(original), value.get<float>());
    }

    return nullptr;
}

void decorate_action(std::unique_ptr<BinaryAction>& original, const nl::json& decorators) {
    for (auto& [key, value] : decorators.items()) {
        auto decorator = create_action_decorator(original, key, value);
        if (!decorator) {
            continue;
        }
        original = std::move(decorator);
    }
}

std::unique_ptr<BinaryAction> create_single_action(const nl::json& json) {
    std::unique_ptr<BinaryAction> res = create_undecorated_action(json);
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

std::unique_ptr<BinaryAction> create_multi_action(const nl::json& json) {
    std::vector<std::unique_ptr<BinaryAction>> actions;
    actions.reserve(json.size());

    for (const auto& [_, value] : json.items()) {
        auto action = create_single_action(value);
        if (action) {
            actions.push_back(std::move(action));
        }
    }

    return std::make_unique<MultiAction>(std::move(actions));
}
}  // namespace

template <>
std::unique_ptr<BinaryAction> Factory::create<BinaryAction>(const nl::json& json) {
    if (json.is_array()) {
        return create_multi_action(json);
    }

    if (!json.is_object()) {
        return nullptr;
    }

    auto actions = json.find("actions");
    if (actions != json.end()) {
        auto res = create_multi_action(*actions);
        auto decorators = json.find("with");
        if (decorators == json.end()) {
            return res;
        }

        decorate_action(res, *decorators);
        return res;
    }

    return create_single_action(json);
}

template <>
std::unique_ptr<Action> Factory::create<Action>(const nl::json& json) {
    return create<BinaryAction>(json);
}
}  // namespace action

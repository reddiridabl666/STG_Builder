#include "ActionFactory.hpp"

#include "ActionTypes.hpp"
#include "Decorators.hpp"

namespace action {

namespace {
std::unique_ptr<PropertyGetter> create_undecorated_getter(const nl::json& json) {
    if (json.is_object()) {
        return create_undecorated_getter(json.at("property"));
    }

    if (json.is_number()) {
        return std::make_unique<ConstGetter>(json.get<float>());
    }

    if (json.is_string()) {
        return std::make_unique<ObjectPropertyGetter>(json.get<std::string>());
    }

    return nullptr;
}

std::unique_ptr<PropertyGetter> create_getter_decorator(std::unique_ptr<PropertyGetter>& original,
                                                        const std::string& key, const nl::json& value) {
    if (key == "mul") {
        return std::make_unique<MultiplyGetterDecorator>(std::move(original), value.get<float>());
    }

    return nullptr;
}

std::unique_ptr<PropertyGetter> create_getter(const nl::json& json) {
    std::unique_ptr<PropertyGetter> res = create_undecorated_getter(json);
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

std::unique_ptr<Action> create_undecorated_action(const nl::json& json) {
    std::string type = json.value("type", "");
    std::string property = json.value("property", "");

    if (property == "") {
        return nullptr;
    }

    if (type == "") {
        return nullptr;
    }

    if (type == "inc") {
        return std::make_unique<Incrementor>(std::move(property));
    }

    if (type == "dec") {
        return std::make_unique<Decrementor>(std::move(property));
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

std::unique_ptr<Action> create_action_decorator(std::unique_ptr<Action>& original, const std::string& key,
                                                const nl::json& value) {
    if (key == "timeout") {
        return std::make_unique<TimeoutDecorator>(std::move(original), value.get<float>());
    }

    return nullptr;
}

std::unique_ptr<Action> create_single_action(const nl::json& json) {
    std::unique_ptr<Action> res = create_undecorated_action(json);
    if (!res) {
        return res;
    }

    auto decorators = json.find("with");
    if (decorators == json.end()) {
        return res;
    }

    for (auto& [key, value] : decorators->items()) {
        auto decorator = create_action_decorator(res, key, value);
        if (!decorator) {
            continue;
        }
        res = std::move(decorator);
    }

    return res;
}
}  // namespace

std::unique_ptr<Action> Factory::create(const nl::json& json) {
    if (json.is_object()) {
        return create_single_action(json);
    }

    if (!json.is_array()) {
        return nullptr;
    }

    std::vector<std::unique_ptr<Action>> actions;
    actions.reserve(json.size());

    for (const auto& [_, value] : json.items()) {
        auto action = create_single_action(value);
        if (action) {
            actions.push_back(std::move(action));
        }
    }

    return std::make_unique<MultiAction>(std::move(actions));
}

}  // namespace action

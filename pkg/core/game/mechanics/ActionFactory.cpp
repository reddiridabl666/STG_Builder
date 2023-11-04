#include "ActionFactory.hpp"

#include "ActionTypes.hpp"

namespace action {

namespace {
std::unique_ptr<Action> create_single(const nl::json& json) {
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

    auto getter = [&value] -> std::unique_ptr<PropertyGetter> {
        if (value->is_number()) {
            return std::make_unique<ConstGetter>(value->get<float>());
        }

        if (value->is_string()) {
            return std::make_unique<ObjectPropertyGetter>(value->get<std::string>());
        }

        return nullptr;
    }();

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
}  // namespace

std::unique_ptr<Action> Factory::create(const nl::json& json) {
    if (json.is_object()) {
        return create_single(json);
    }

    if (!json.is_array()) {
        return nullptr;
    }

    std::vector<std::unique_ptr<Action>> actions;
    actions.reserve(json.size());

    for (const auto& [_, value] : json.items()) {
        auto action = create_single(value);
        if (action) {
            actions.push_back(std::move(action));
        }
    }

    return std::make_unique<MultiAction>(std::move(actions));
}

}  // namespace action

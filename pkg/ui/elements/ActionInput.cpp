#include "ActionInput.hpp"

#include "Combo.hpp"
#include "ImguiUtils.hpp"
#include "Map.hpp"
#include "Messages.hpp"

namespace ui {
void from_json(const nl::json& json, SingleActionInput::Value& value) {
    if (json.is_string()) {
        value.type = SingleActionInput::Value::Type::Property;
        value.property = json.get<std::string>();
        return;
    }

    if (json.is_number()) {
        value.type = SingleActionInput::Value::Type::Const;
        value.value = json.get<float>();
        return;
    }

    value.type = SingleActionInput::Value::Type::Property;
    value.property = json.value("property", "score");
    value.with = json.value("with", decltype(value.with){});
}

void to_json(nl::json& json, const SingleActionInput::Value& value) {
    if (value.with.empty()) {
        if (value.type == SingleActionInput::Value::Type::Property) {
            json = value.property;
        } else {
            json = value.value;
        }
        return;
    }

    json["property"] = value.property;
    json["with"] = value.with;
}

void from_json(const nl::json& json, SingleActionInput& input) {
    input.value = json.value("value", SingleActionInput::Value{});
    input.property = json.value("property", "score");
    input.action = json.value("action", "add");
    input.with = json.value("with", decltype(input.with){});
}

static const std::vector<std::string> no_property_types = {"die"};
static const std::vector<std::string> non_value_types = {"reset", "inc", "dec", "shoot"};

void to_json(nl::json& json, const SingleActionInput& input) {
    json["action"] = input.action;

    if (std::ranges::find(no_property_types, input.action) == no_property_types.end()) {
        json["property"] = input.property;
    }

    if (std::ranges::find(non_value_types, input.action) == non_value_types.end()) {
        json["value"] = input.value;
    }

    if (!input.with.empty()) {
        json["with"] = input.with;
    }
}

void from_json(const nl::json& json, ActionInput& input) {
    if (json.is_array()) {
        input.actions = json.get<std::vector<SingleActionInput>>();
        return;
    }

    auto actions = json.find("actions");

    if (actions == json.end()) {
        input.actions.front() = json.get<SingleActionInput>();
        return;
    }

    input.actions = actions->get<std::vector<SingleActionInput>>();
    input.with = json.value("with", decltype(input.with){});
}

void to_json(nl::json& json, const ActionInput& input) {
    if (input.actions.empty()) {
        json = nl::json::object();
        return;
    }

    if (input.actions.size() == 1) {
        json = input.actions.front();
        return;
    }

    json["actions"] = input.actions;
    json["with"] = input.with;
}

void SingleActionInput::Value::draw() {
    static const std::vector<std::string> types = {"const", "property"};

    ImGui::Combo(message(Message::Type), (int*)&type, types);

    if (type == Type::Property) {
        if (ImGui::TreeNode(message(Message::Value))) {
            ImGui::InputText(message(Message::Property), &property);
            if (ImGui::TreeNode(message(Message::Decorators))) {
                draw_map(with, new_deco_);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::InputFloat(message(Message::Value), &value);
    }
}

void SingleActionInput::draw() {
    static const std::vector<std::string> types = {"set",   "add", "sub", "mul", "div",
                                                   "reset", "inc", "dec", "die", "shoot"};

    if (std::ranges::find(no_property_types, action) == no_property_types.end()) {
        ImGui::InputText(message(Message::Property), &property);
    }

    action_id_ = combo::get_item_id(types, action);
    if (ImGui::Combo(message(Message::ActionType), &action_id_, types)) {
        action = combo::find_item(types, action_id_);
    }

    if (std::ranges::find(non_value_types, action) == non_value_types.end()) {
        ImGui::PushID(&value);
        value.draw();
        ImGui::PopID();
    }

    if (ImGui::TreeNode(message(Message::Decorators))) {
        draw_map(with, new_deco_);
        ImGui::TreePop();
    }
}

ActionInput::ActionInput() : actions{SingleActionInput{}} {}

void ActionInput::draw() {
    for (size_t idx = 0; idx < actions.size(); ++idx) {
        actions[idx].draw();
        ImGui::PushID(&actions[idx]);
        if (actions.size() > 0 && ImGui::Button(message(Message::Delete))) {
            to_delete_ = idx;
        }
        ImGui::PopID();
    }

    if (ImGui::Button(message(Message::Add))) {
        actions.push_back(SingleActionInput{});
    }

    if (to_delete_ != -1) {
        actions.erase(actions.begin() + to_delete_);
        to_delete_ = -1;
    }
}
}  // namespace ui

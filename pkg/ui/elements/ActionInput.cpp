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

void SingleActionInput::Value::draw() {
    static const std::vector<std::string> types = {"const", "property"};

    ImGui::Combo(message(Message::Type), (int*)&type, types);

    if (type == Type::Property) {
        ImGui::InputText(message(Message::Property), &property);
    } else {
        ImGui::InputFloat(message(Message::Value), &value);
    }

    draw_map(with, message(Message::Decorators), new_deco_);
}

void SingleActionInput::draw() {
    static const std::vector<std::string> types = {"set", "add", "sub", "mul", "div", "reset", "inc", "dec"};

    ImGui::InputText(message(Message::Property), &property);

    action_id_ = combo::get_item_id(types, action);
    if (ImGui::Combo(message(Message::ActionType), &action_id_, types)) {
        action = combo::find_item(types, action_id_);
    }
    value.draw();

    draw_map(with, message(Message::Decorators), new_deco_);
}

void ActionInput::draw() {
    for (size_t idx = 0; idx < actions.size(); ++idx) {
        actions[idx].draw();
        ImGui::PushID(&actions[idx]);
        if (ImGui::Button(message(Message::Delete))) {
            to_delete_ = idx;
        }
        ImGui::PopID();
    }

    if (to_delete_ != -1) {
        actions.erase(actions.begin() + to_delete_);
        to_delete_ = -1;
    }
}
}  // namespace ui

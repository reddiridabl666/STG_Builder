#include "TimedActionInput.hpp"

#include "ImguiUtils.hpp"
#include "Messages.hpp"

namespace ui {
void TimedActionInput::draw() {
    ActionInput::draw();

    ImGui::InputFloat(message(Message::Delay), &delay);
    ImGui::InputFloat(message(Message::Timeout), &timeout);
    ImGui::Checkbox(message(Message::Once), &once);
}

void from_json(const nl::json& json, TimedActionInput& input) {
    from_json(json, static_cast<ActionInput&>(input));
    input.delay = json.value("delay", 0.f);
    input.timeout = json.value("timeout", 0.f);
    input.once = json.value("once", false);
}

void to_json(nl::json& json, const TimedActionInput& input) {
    to_json(json, static_cast<const ActionInput&>(input));
    if (input.once) {
        json["once"] = input.once;
    }

    json["delay"] = input.delay;
    json["timeout"] = input.timeout;
}

void TimedActionListInput::draw() {
    size_t i = 0;

    for (auto& input : list_) {
        ImGui::PushID(&input);
        input.draw();
        if (ImGui::Button(message(Message::Delete))) {
            to_delete_ = i;
        }
        ++i;
        ImGui::PopID();
    }

    if (ImGui::Button(message(Message::Add))) {
        list_.push_back(TimedActionInput{});
    }

    if (to_delete_ != -1) {
        list_.erase(list_.begin() + to_delete_);
        to_delete_ = -1;
    }
}

void from_json(const nl::json& json, TimedActionListInput& list) {
    if (!json.is_array()) {
        return;
    }

    list.list_.reserve(json.size());
    for (auto& [_, value] : json.items()) {
        list.list_.push_back(value.get<TimedActionInput>());
    }
}

void to_json(nl::json& json, const TimedActionListInput& list) {
    for (auto& item : list.list_) {
        json.push_back(item);
    }
}
}  // namespace ui

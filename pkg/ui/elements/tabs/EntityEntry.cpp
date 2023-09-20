#include "EntityEntry.hpp"

namespace ui {
const std::unordered_set<std::string> EntityEntry::kBaseValues = {"image", "size", "tag", "speed", ""};

EntityEntry::EntityEntry(const std::string& name, nl::json& json, std::shared_ptr<sf::Texture>&& image)
    : texture_(std::move(image)), name(name), old_name_(name) {
    nl::from_json(json, *this);

    for (auto& [key, val] : json.items()) {
        if (!kBaseValues.contains(key) && val.is_number()) {
            values_[key] = val.template get<float>();
        }
    }
}

nl::json EntityEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);

    for (const auto& [key, val] : values_) {
        res[key] = val;
    }

    return res;
}

void EntityEntry::draw(const Window&) {
    ImGui::BeginGroupPanel(name.c_str());

    ImGui::Image(*texture_, sf::Vector2f{50, 50});
    ImGui::SameLine();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
    ImGui::Text(description.c_str());

    ImGui::EndGroupPanel();

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (ImGui::IsItemClicked()) {
        shown_ = true;
    }

    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);
    if (ImGui::Button(message(Message::Delete))) {
        to_delete_ = true;
    }

    if (shown_) {
        ImGui::Begin(old_name_.c_str(), &shown_, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::InputText(message(Message::Name), &name);
        ImGui::InputText(message(Message::Desc), &description);
        ImGui::InputText(message(Message::Image), &image);
        ImGui::InputText(message(Message::Tag), &tag);
        ImGui::InputFloat(message(Message::Speed), &speed);

        ImGui::SizeInput(message(Message::Size), &size.x, &size.y);

        ImGui::SeparatorText(message(Message::Stats));
        for (auto& [key, val] : values_) {
            ImGui::InputFloat(key.c_str(), &val);
        }

        ImGui::InputText("##stat_name", &stat_name_);
        ImGui::SameLine();
        if (ImGui::Button(message(Message::AddStat)) && !kBaseValues.contains(stat_name_)) {
            values_.emplace(stat_name_, 0);
            stat_name_ = "";
        }

        ImGui::End();

        if (!shown_) {
            old_name_ = name;
            // TODO: Save object type somehow
        }
    }
}
}  // namespace ui

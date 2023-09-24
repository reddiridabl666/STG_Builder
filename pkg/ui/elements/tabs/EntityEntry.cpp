#include "EntityEntry.hpp"

#include <imgui-SFML.h>

#include "Bus.hpp"

namespace ui {
const std::unordered_set<std::string> EntityEntry::kBaseValues = {"image", "size", "tag", "speed", ""};

EntityEntry::EntityEntry(const std::string& name, nl::json* json, std::shared_ptr<sf::Texture>&& image)
    : texture_(std::move(image)), name(name), old_name_(name), stats_(kBaseValues), data_(json) {
    nl::from_json(*json, *this);
    stats_.from_json(*json);
}

nl::json EntityEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);

    stats_.to_json(res);

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

        stats_.draw();

        ImGui::End();

        if (!shown_) {
            old_name_ = name;
            *data_ = to_json();
            // Bus::get().emit(Bus::Event::ObjectTypesChanged, data_);
        }
    }
}
}  // namespace ui

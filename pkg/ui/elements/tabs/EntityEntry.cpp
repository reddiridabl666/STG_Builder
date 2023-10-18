#include "EntityEntry.hpp"

#include <imgui-SFML.h>

#include "Bus.hpp"
#include "Combo.hpp"

static const char* object_tags = "enemy\0player\0bullet";

namespace ui {
const std::unordered_set<std::string> EntityEntry::kBaseValues = {"image", "size", "tag", "speed", ""};

EntityEntry::EntityEntry(const std::string& name, nl::json* json, std::shared_ptr<sf::Texture>&& image,
                         size_t obj_count)
    : texture_(std::move(image)),
      name(name),
      old_name_(name),
      stats_(kBaseValues),
      data_(json),
      obj_count_(obj_count) {
    nl::from_json(*json, *this);
    tag_id = combo::get_item_id(object_tags, tag);
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

    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

    if (ImGui::Button(message(Message::NewObject))) {
        Bus::get().emit(Bus::Event::ObjectCreated, name);
        ++obj_count_;
    }

    if (ImGui::Button(message(Message::Delete))) {
        if (obj_count_ != 0) {
            ImGui::OpenPopup(message(Message::CantDelete));
        } else {
            to_delete_ = true;
        }
    }
    ImGui::EndGroup();

    if (ImGui::BeginPopup(message(Message::CantDelete))) {
        ImGui::Text(message(Message::CantDeleteFull), std::to_string(obj_count_).c_str());
        ImGui::EndPopup();
    }

    if (shown_) {
        ImGui::Begin(old_name_.c_str(), &shown_, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::InputText(message(Message::Name), &name);
        ImGui::InputText(message(Message::Desc), &description);
        ImGui::InputText(message(Message::Image), &image);

        if (ImGui::Combo(message(Message::Tag), &tag_id, object_tags)) {
            tag = combo::find_item(object_tags, tag_id);
        }

        ImGui::InputFloat(message(Message::Speed), &speed);
        ImGui::SizeInput(message(Message::Size), &size.x, &size.y);

        stats_.draw();

        ImGui::End();

        if (!shown_) {
            old_name_ = name;
            *data_ = to_json();
            Bus::get().emit(Bus::Event::ObjectTypeChanged, *data_);
        }
    }
}
}  // namespace ui

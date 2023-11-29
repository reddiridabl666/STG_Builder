#include "EntityEntry.hpp"

#include <imgui-SFML.h>

#include "Bus.hpp"
#include "Combo.hpp"
#include "FuncInput.hpp"

namespace {
static const std::vector<std::string> object_tags = {"enemy", "player", "enemy_bullet", "player_bullet"};
}  // namespace

namespace ui {
const std::unordered_set<std::string> EntityEntry::kBaseValues = {
    "image",         "size",   "tag",        "speed", "patterns", "on_player_actions", "on_character_death",
    "on_own_action", "hitbox", "description"};

EntityEntry::EntityEntry(const std::string& name, nl::json* json, std::shared_ptr<sf::Texture>&& image,
                         size_t obj_count)
    : texture_(std::move(image)), name(name), stats_(kBaseValues), old_name_(name), data_(json), obj_count_(obj_count) {
    nl::from_json(*json, *this);
    tag_id = combo::get_item_id(object_tags, tag);
    stats_.from_json(*json);
    outline_color_ = hitbox.outline_color;
    fill_color_ = hitbox.fill_color;
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
        Bus<std::string>::get().emit(tag == "player" ? Event::PlayerCreated : Event::ObjectCreated, name);
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

        ImGui::NewLine();

        if (ImGui::CollapsingHeader(message(Message::Hitbox))) {
            ImGui::PushID(&hitbox);

            static const std::vector<std::string> hitbox_types = {"rect", "circle", "none"};
            ImGui::Combo("##hitbox_type", (int*)&hitbox.type, hitbox_types);

            switch (hitbox.type) {
                case Hitbox::Type::Rect:
                    ImGui::SizeInput(message(Message::Size), &hitbox.size.x, &hitbox.size.y);
                    break;
                case Hitbox::Type::Circle:
                    ImGui::InputFloat(message(Message::Radius), &hitbox.radius);
                    break;
                default:
                    break;
            }

            ImGui::Checkbox(message(Message::Shown), &hitbox.shown);
            if (hitbox.shown) {
                ImGui::InputInt(message(Message::Outline), &hitbox.outline);

                ImGui::ColorEdit4(message(Message::OutlineColor), &outline_color_.x);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    hitbox.outline_color = outline_color_;
                }

                ImGui::ColorEdit4(message(Message::FillColor), &fill_color_.x);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    hitbox.fill_color = fill_color_;
                }
            }

            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader(message(Message::IsAlive))) {
            AliveFuncInput(lives);
        }

        collision.draw(message(Message::Collisions));

        on_player_action.draw(message(Message::OnPlayerActions));

        on_own_action.draw(message(Message::OnOwnActions));

        on_character_death.draw(message(Message::OnCharacterDeath));

        if (ImGui::CollapsingHeader(message(Message::TimedActions))) {
            timed_actions.draw();
        }

        if (ImGui::CollapsingHeader(message(Message::Patterns))) {
            draw_map(patterns, new_pattern_);
        }

        ImGui::End();

        if (!shown_) {
            old_name_ = name;

            *data_ = to_json();

            auto tmp = *data_;
            tmp["name"] = name;

            Bus<nl::json>::get().emit(Event::ObjectTypeChanged, tmp);
        }
    }
}
}  // namespace ui

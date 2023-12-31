#include "ObjectEntry.hpp"

#include "Combo.hpp"
#include "EditorWindow.hpp"
#include "ui/elements/FuncInput.hpp"
#include "ui/elements/KeyMapInput.hpp"

namespace ui {
nl::json CommonEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);
    stats.to_json(res);

    if (activity_start == kDefaultActivityStart) {
        res.erase("activity_start");
    }

    if (move.rules.empty()) {
        res.erase("move");
    }

    return res;
}

ObjectEntry::Changes CommonEntry::draw(const std::string& obj_types) {
    Changes changes;

    type_id = combo::get_item_id(obj_types, type);

    if (ImGui::Combo(message(Message::ObjectType), &type_id, obj_types.c_str())) {
        type = combo::find_item(obj_types, type_id);
        changes.type = true;
    }

    if (ImGui::InputInt(message(Message::Rotation), &rotation)) {
        changes.rotation = true;
    }

    ImGui::InputText(message(Message::ActivityStart), &activity_start);
    if (ImGui::BeginItemTooltip()) {
        ImGui::Text(message(Message::ActivityStartHint));
        ImGui::EndTooltip();
    }

    pos.draw(message(Message::Pos));
    if (ImGui::IsItemDeactivated()) {
        changes.pos = true;
    }

    int to_delete = MoveFuncInput(move);
    if (to_delete != -1) {
        move.rules.erase(move.rules.begin() + to_delete);
    }

    AliveFuncInput(lives);

    ImGui::Checkbox(message(Message::StopAtBounds), &stop_at_bounds);

    stats.draw();

    return changes;
}

nl::json PlayerEntry::to_json() const {
    nl::json res;

    nl::to_json(res, *this);
    stats.to_json(res);
    return res;
}

ObjectEntry::Changes PlayerEntry::draw(const std::string& obj_types) {
    Changes changes;

    type_id = combo::get_item_id(obj_types, type);

    if (ImGui::Combo(message(Message::ObjectType), &type_id, obj_types.c_str())) {
        type = combo::find_item(obj_types, type_id);
        changes.type = true;
    }

    if (ImGui::InputInt(message(Message::Rotation), &rotation)) {
        changes.rotation = true;
    }

    pos.draw(message(Message::Pos));
    if (ImGui::IsItemDeactivated()) {
        changes.pos = true;
    }

    ImGui::Checkbox(message(Message::StopAtBounds), &stop_at_bounds);

    stats.draw();

    ImGui::SeparatorText(message(Message::Controls));
    KeyMapInput::draw(opts.keys, builder::EditorWindow::get());
    return changes;
}
}  // namespace ui

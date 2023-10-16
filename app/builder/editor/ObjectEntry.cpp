#include "ObjectEntry.hpp"

#include "ui/elements/FuncInput.hpp"

namespace ui {

namespace {
std::string find_item(const std::string& input, int idx) {
    int cur = 0;
    int i = 0;

    for (auto chr : input) {
        if (cur == idx) {
            return input.substr(i, input.substr(i).find('\0'));
        }

        if (chr == '\0') {
            ++cur;
        }

        ++i;
    }
    return "";
}

int get_type_id(const std::string& types, const std::string& to_find) {
    return std::ranges::count(types.substr(0, types.find(to_find)), '\0');
}
}  // namespace

nl::json CommonEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);
    stats.to_json(res);

    if (activity_start == kDefaultActivityStart) {
        res.erase("activity_start");
    }

    return res;
}

std::optional<ObjectEntry::VisualDelta> CommonEntry::draw(const std::string& obj_types) {
    bool had_changes = false;

    type_id = get_type_id(obj_types, type);

    if (ImGui::Combo(message(Message::ObjectType), &type_id, obj_types.c_str())) {
        type = find_item(obj_types, type_id);
    }

    if (ImGui::InputInt(message(Message::Rotation), &rotation)) {
        had_changes = true;
    }

    ImGui::InputText(message(Message::ActivityStart), &activity_start);
    if (ImGui::BeginItemTooltip()) {
        ImGui::Text(message(Message::ActivityStartHint));
        ImGui::EndTooltip();
    }

    pos.draw(message(Message::Pos));
    if (ImGui::IsItemDeactivated()) {
        had_changes = true;
    }

    MoveFuncInput(move);
    AliveFuncInput(lives);

    stats.draw();

    return had_changes ? std::optional<VisualDelta>(VisualDelta{pos, rotation}) : std::nullopt;
}

nl::json PlayerEntry::to_json() const {
    nl::json res;

    nl::to_json(res, *this);
    stats.to_json(res);
    return res;
}

std::optional<ObjectEntry::VisualDelta> PlayerEntry::draw(const std::string& obj_types) {
    bool had_changes = false;

    type_id = get_type_id(obj_types, type);

    if (ImGui::Combo(message(Message::ObjectType), &type_id, obj_types.c_str())) {
        type = find_item(obj_types, type_id);
    }

    if (ImGui::InputInt(message(Message::Rotation), &rotation)) {
        had_changes = true;
    }

    pos.draw(message(Message::Pos));
    if (ImGui::IsItemDeactivated()) {
        had_changes = true;
    }

    stats.draw();

    return had_changes ? std::optional<VisualDelta>(VisualDelta{pos, rotation}) : std::nullopt;
}
}  // namespace ui

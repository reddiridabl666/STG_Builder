#include "ObjectEditor.hpp"

#include <ranges>

#include "FuncBuilder.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "ui/common/Bus.hpp"
#include "ui/elements/FuncInput.hpp"

namespace ui {
namespace {
std::string get_obj_types(const nl::json& json) {
    std::string res = "";

    for (auto& [key, _] : json.items()) {
        res += key + '\0';
    }

    return res;
}

std::string find_item(std::string input, int idx) {
    int cur = 0;
    for (auto chr : input) {
        if (cur == idx) {
            return input.substr(cur, input.substr(cur).find('\0'));
        }

        if (chr == '\0') {
            ++cur;
        }
    }
    return "";
}

int get_type_id(const std::string& types, const std::string& to_find) {
    return std::ranges::count(types.substr(0, types.find(to_find)), '\0');
}
}  // namespace

ObjectEditor::ObjectEditor(Window& window, builder::EditableGame& game, nl::json& data,
                           const nl::json& entities)
    : window_(window), data_(data), game_(game), obj_types_(get_obj_types(entities)) {
    window.add_handler("obj_editor", sf::Event::MouseButtonPressed, [&window, this](const sf::Event& event) {
        if (event.mouseButton.button != sf::Mouse::Right) {
            return;
        }

        auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

        auto obj = game_.object_by_pos(coords);
        if (!obj) {
            return;
        }

        auto json = data_["entities"][obj->props().at(builder::kJsonID).get()];
        auto entry = ObjectEntry::from_json(json);

        entry.type_id = get_type_id(obj_types_, entry.type);

        shown_.emplace(obj, std::move(entry));
    });

    Bus::get().on(Bus::Event::ObjectTypesChanged, "obj_editor", [this](const nl::json& data) {
        obj_types_ = get_obj_types(data);
        for (auto& [_, obj_data] : shown_) {
            obj_data.type_id = get_type_id(obj_types_, obj_data.type);
        }
    });
}

void ObjectEditor::draw(const Window&) {
    if (game_.object_by_pos(window_.pixel_to_coords(sf::Mouse::getPosition(window_.base())))) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    for (auto it = shown_.begin(); it != shown_.end();) {
        auto next = std::next(it);
        auto& [obj, obj_data] = *it;

        bool open = true;

        ImGui::Begin(obj->name().c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PushItemWidth(100);

        ImGui::Combo(message(Message::ObjectType), &obj_data.type_id, obj_types_.c_str());
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            obj_data.type = find_item(obj_types_, obj_data.type_id);
        }
        ImGui::InputInt(message(Message::Rotation), &obj_data.rotation);

        ImGui::InputText(message(Message::ActivityStart), &obj_data.activity_start);
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text(message(Message::ActivityStartHint));
            ImGui::EndTooltip();
        }

        obj_data.pos.draw(message(Message::Pos));

        MoveFuncInput(obj_data.move);
        AliveFuncInput(obj_data.lives);

        obj_data.stats.draw();

        ImGui::PopItemWidth();

        ImGui::End();

        if (!open) {
            data_["entries"][obj->props().at(builder::kJsonID).get()] = obj_data.to_json();
            shown_.erase(it);
        }

        it = next;
    }
}

ObjectEditor::~ObjectEditor() {
    Bus::get().off(Bus::Event::ObjectTypesChanged, "obj_editor");
    window_.remove_handler("obj_editor");
}

ObjectEditor::ObjectEntry ObjectEditor::ObjectEntry::from_json(const nl::json& json) {
    auto res = json.template get<ObjectEntry>();
    res.stats.from_json(json);
    return res;
}

nl::json ObjectEditor::ObjectEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);
    stats.to_json(res);
    return res;
}
}  // namespace ui

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
    window.add_handler("obj_editor_click", sf::Event::MouseButtonPressed,
                       [&window, this](const sf::Event& event) {
                           auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

                           auto obj = game_.object_by_pos(coords);
                           if (!obj) {
                               return;
                           }

                           switch (event.mouseButton.button) {
                               case sf::Mouse::Left:
                                   drag_n_drop_ = true;
                                   drag_target_ = obj;
                                   return;
                               case sf::Mouse::Right:
                                   break;
                               default:
                                   return;
                           }

                           auto json = data_.at("entities").at(obj->props().at(builder::kJsonID).get());

                           fmt::println("Id: {}", obj->props().at(builder::kJsonID).get());
                           fmt::println("Entities length: {}", data_.at("entities").size());
                           fmt::println("Entities: {}", data_.at("entities").dump(4));
                           fmt::println("json: {}", json.dump(4));

                           auto entry = ObjectEntry::from_json(json);

                           entry.type_id = get_type_id(obj_types_, entry.type);

                           shown_.emplace(obj, std::move(entry));
                       });

    window.add_handler("obj_editor_release", sf::Event::MouseButtonReleased, [this](const sf::Event& event) {
        if (event.mouseButton.button != sf::Mouse::Left) {
            return;
        }

        drag_n_drop_ = false;
        drag_target_ = nullptr;
    });

    Bus::get().on(Bus::Event::ObjectTypesChanged, "obj_editor_changed", [this](const nl::json& data) {
        obj_types_ = get_obj_types(data);
        for (auto& [_, obj_data] : shown_) {
            obj_data.type_id = get_type_id(obj_types_, obj_data.type);
        }
    });

    Bus::get().on(Bus::Event::ObjectCreated, "obj_editor_created", [this](const nl::json& data) {
        if (!data.is_string()) {
            return;
        }
        auto type = data.template get<std::string>();

        auto& obj = game_.new_object(type);
        obj.props().set(builder::kJsonID, data_.at("entities").size());

        data_.at("entities")
            .push_back({
                {"type", type},
                {"pos", window_.get_view().getCenter()},
            });
    });
}

void ObjectEditor::draw(const Window&) {
    auto mouse_pos = window_.pixel_to_coords(sf::Mouse::getPosition(window_.base()));

    if (drag_n_drop_) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        game_.set_object_pos(*drag_target_, mouse_pos);
    } else if (game_.object_by_pos(mouse_pos)) {
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
            data_["entities"][obj->props().at(builder::kJsonID).get()] = obj_data.to_json();
            shown_.erase(it);
        }

        it = next;
    }
}

ObjectEditor::~ObjectEditor() {
    Bus::get().off(Bus::Event::ObjectTypesChanged, "obj_editor");
    window_.remove_handler("obj_editor_click");
    window_.remove_handler("obj_editor_release");
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

    if (count == -1) {
        res.erase("count");
        res.erase("delta");
    }

    if (activity_start == kDefaultActivityStart) {
        res.erase("activity_start");
    }

    return res;
}
}  // namespace ui

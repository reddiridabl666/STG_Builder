#include "ObjectEditor.hpp"

#include <ranges>

#include "FuncBuilder.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "ObjectEntryFactory.hpp"
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
}  // namespace

ObjectEditor::ObjectEditor(Window& window, builder::EditableGame& game, nl::json& level_data, nl::json& game_data,
                           const nl::json& entities)
    : window_(window),
      level_data_(level_data),
      game_data_(game_data),
      game_(game),
      obj_types_(get_obj_types(entities)) {
    window.add_handler("obj_editor_click", sf::Event::MouseButtonPressed, [&window, this](const sf::Event& event) {
        auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

        auto obj = game_.get_object(coords);
        if (!obj) {
            return;
        }

        switch (event.mouseButton.button) {
            case sf::Mouse::Left:
                drag_n_drop_ = true;
                drag_target_ = obj;
                drag_pos_ = obj->pos();
                return;
            case sf::Mouse::Right:
                break;
            default:
                return;
        }

        auto json = json_by_obj(*obj);
        auto entry = ObjectEntryFactory::create(json, obj->tag());

        shown_.emplace(obj, std::move(entry));
    });

    window.add_handler("obj_editor_release", sf::Event::MouseButtonReleased, [this](const sf::Event& event) {
        if (!drag_n_drop_ || event.mouseButton.button != sf::Mouse::Left) {
            return;
        }

        update_dragged_obj_pos();
        drag_n_drop_ = false;
    });

    window.add_handler("obj_editor_undo", sf::Event::KeyPressed, [this](const sf::Event& event) {
        if (!drag_target_) {
            return;
        }

        if (event.key.code == sf::Keyboard::Z && event.key.control) {
            game_.set_object_pos(*drag_target_, drag_pos_);
            update_dragged_obj_pos();
            drag_target_.reset();
            drag_n_drop_ = false;
        }
    });

    Bus<nl::json>::get().on(Event::ObjectTypesChanged, "obj_editor_changed", [this](const nl::json& data) {
        obj_types_ = get_obj_types(data);
    });

    Bus<std::string>::get().on(Event::ObjectCreated, "obj_editor_created", [this](const std::string& type) {
        auto& obj = game_.new_object(type);
        obj.props().set(builder::kJsonID, level_data_.at("entities").size());

        level_data_.at("entities")
            .push_back({
                {"type", type},
                {"pos", window_.get_view().getCenter()},
            });
    });

    Bus<std::string>::get().on(Event::ObjectTypeCreated, "obj_editor_type_created", [this](const std::string& name) {
        game_.new_object_type(name);
    });
}

void ObjectEditor::draw(const Window&) {
    auto mouse_pos = window_.pixel_to_coords(sf::Mouse::getPosition(window_.base()));

    if (drag_n_drop_) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        game_.set_object_pos(*drag_target_, mouse_pos);
    } else if (game_.get_object(mouse_pos)) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    for (auto it = shown_.begin(); it != shown_.end();) {
        auto next = std::next(it);
        auto& [obj, obj_data] = *it;

        bool open = true;

        ImGui::Begin(obj->name().c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PushItemWidth(100);

        auto changes = obj_data->draw(obj_types_);

        if (changes.type) {
            json_by_obj(*obj) = obj_data->to_json();

            game_.reload_object(obj->name(), *ObjectOptionsFactory(game_.field()).generate(obj_data->to_json()));
            shown_.erase(it);
            it = next;
            ImGui::End();
            continue;
        } else if (changes.rotation) {
            obj->set_rotation(obj_data->get_rotation());
            game_.set_object_pos(*obj, obj_data->get_pos().to_vec(game_.field()));
        } else if (changes.pos) {
            game_.set_object_pos(*obj, obj_data->get_pos().to_vec(game_.field()));
        }

        ImGui::PopItemWidth();

        auto btn_label = std::string(message(Message::Delete)) + "##" + obj->name();
        if (ImGui::Button(btn_label.c_str())) {
            Bus<std::string>::get().emit(Event::ObjectDeleted, obj->name().substr(0, obj->name().rfind('-')));

            erase_obj(*obj);
            shown_.erase(it);
        }

        ImGui::End();

        if (!open) {
            json_by_obj(*obj) = obj_data->to_json();
            shown_.erase(it);
        }

        it = next;
    }
}

ObjectEditor::~ObjectEditor() {
    Bus<nl::json>::get().off(Event::ObjectTypesChanged, "obj_editor_changed");
    Bus<std::string>::get().off(Event::ObjectCreated, "obj_editor_created");
    Bus<std::string>::get().off(Event::ObjectTypeCreated, "obj_editor_type_created");

    window_.remove_handler("obj_editor_click");
    window_.remove_handler("obj_editor_release");
    window_.remove_handler("obj_editor_undo");
}

nl::json& ObjectEditor::json_by_obj(const GameObject& obj) {
    if (obj.tag() == GameObject::Tag::Player) {
        return game_data_.at("players").at(obj.props().at(engine::kPlayerNum));
    }
    return level_data_.at("entities").at(obj.props().at(builder::kJsonID));
}

void ObjectEditor::erase_obj(const GameObject& obj) {
    if (obj.tag() == GameObject::Tag::Player) {
        game_data_.at("players").erase(obj.props().at(engine::kPlayerNum));
    } else {
        level_data_.at("entities").erase(obj.props().at(builder::kJsonID));
    }
    game_.remove_object(obj.name());
}

void ObjectEditor::update_dragged_obj_pos() {
    auto it = shown_.find(drag_target_);
    if (it != shown_.end()) {
        it->second->set_pos(drag_target_->pos());
    }
    json_by_obj(*drag_target_)["pos"] = drag_target_->pos();
}
}  // namespace ui

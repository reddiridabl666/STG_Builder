#pragma once

#include <unordered_set>

#include "EditableGame.hpp"
#include "FuncInfo.hpp"
#include "GameObject.hpp"
#include "Life.hpp"
#include "Movement.hpp"
#include "UiElement.hpp"

namespace ui {
class ObjectEditor : public Element {
  public:
    ObjectEditor(Window& window, builder::EditableGame& game, nl::json& data)
        : window_(window), game_(game), data_(data) {
        window.add_handler(
            "obj_editor", sf::Event::MouseButtonPressed, [&window, this](const sf::Event& event) {
                auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

                auto obj = game_.object_by_pos(coords);
                if (!obj) {
                    return;
                }

                shown_.emplace(obj, data_["entities"][obj->props().at(builder::kJsonID).get()]);
            });
    }

    void draw(const Window&) override {
        if (game_.object_by_pos(window_.pixel_to_coords(sf::Mouse::getPosition(window_.base())))) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        for (auto it = shown_.begin(); it != shown_.end();) {
            auto next = std::next(it);
            auto& [obj, obj_data] = *it;

            bool open = true;

            ImGui::Begin(obj->name().c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::InputText(message(Message::ObjectType), &obj_data.type);
            ImGui::End();

            if (!open) {
                shown_.erase(it);
            }

            it = next;
        }
    }

    ~ObjectEditor() override {
        window_.remove_handler("obj_editor");
    }

  private:
    struct ObjectEntry {
        ObjectEntry() = default;
        explicit ObjectEntry(const nl::json& json);

        nl::json to_json() const;

        int count = -1;
        sf::Vector2f delta = {};

        std::string type = "object";
        std::string activity_start = "0";
        sf::Vector2<std::string> pos = {};
        int rotation = 0;

        movement::Func move = movement::no_op;
        alive::update lives = alive::default_func;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ObjectEntry, count, delta, type, activity_start, pos,
                                                    rotation);
    };

    Window& window_;
    std::unordered_map<GameObject*, ObjectEntry> shown_;
    nl::json& data_;
    builder::EditableGame& game_;
};
}  // namespace ui

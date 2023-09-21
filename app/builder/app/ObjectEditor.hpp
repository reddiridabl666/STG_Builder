#pragma once

#include <unordered_set>

#include "EditableGame.hpp"
#include "GameObject.hpp"
#include "UiElement.hpp"

namespace ui {
class ObjectEditor : public Element {
  public:
    ObjectEditor(Window& window, builder::EditableGame& game) : window_(window), game_(game) {
        window.add_handler("obj_editor", sf::Event::MouseButtonPressed,
                           [&window, this](const sf::Event& event) {
                               auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

                               auto obj = game_.object_by_pos(coords);
                               if (!obj) {
                                   return;
                               }

                               shown_.insert(obj);
                           });
    }

    void draw(const Window&) override {
        if (game_.object_by_pos(window_.pixel_to_coords(sf::Mouse::getPosition(window_.base())))) {
        }

        for (auto it = shown_.begin(); it != shown_.end();) {
            auto next = std::next(it);
            auto obj = *it;

            bool open = true;

            ImGui::Begin(obj->name().c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text(obj->name().c_str());
            ImGui::End();

            if (!open) {
                shown_.erase(obj);
            }

            it = next;
        }
    }

    ~ObjectEditor() override {
        window_.remove_handler("obj_editor");
    }

  private:
    Window& window_;
    std::unordered_set<GameObject*> shown_;
    builder::EditableGame& game_;
};
}  // namespace ui

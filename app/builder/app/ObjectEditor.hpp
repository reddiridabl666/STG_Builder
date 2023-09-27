#pragma once

#include <unordered_set>

#include "EditableGame.hpp"
#include "GameObject.hpp"
#include "UiElement.hpp"

namespace ui {
class ObjectEditor : public Element {
  public:
    ObjectEditor(Window& window, builder::EditableGame& game) : window_(window), game_(game) {
        window.add_handler("obj_editor_click", sf::Event::MouseButtonPressed,
                           [&window, this](const sf::Event& event) {
                               auto coords = window.pixel_to_coords(event.mouseButton.x, event.mouseButton.y);

                               auto obj = game_.object_by_pos(coords);
                               if (!obj) {
                                   return;
                               }

                               switch (event.mouseButton.button) {
                                   case sf::Mouse::Right:
                                       shown_.insert(obj);
                                       return;
                                   case sf::Mouse::Left:
                                       drag_n_drop_ = true;
                                       drag_target_ = obj;
                                       return;
                                   default:
                                       return;
                               }
                           });

        window.add_handler("obj_editor_release", sf::Event::MouseButtonReleased,
                           [this](const sf::Event& event) {
                               if (event.mouseButton.button != sf::Mouse::Left) {
                                   return;
                               }

                               drag_n_drop_ = false;
                               drag_target_ = nullptr;
                           });
    }

    void draw(const Window&) override {
        auto mouse_pos = window_.pixel_to_coords(sf::Mouse::getPosition(window_.base()));

        if (drag_n_drop_) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            game_.set_object_pos(*drag_target_, mouse_pos);
        } else if (game_.object_by_pos(mouse_pos)) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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
        window_.remove_handler("obj_editor_click");
        window_.remove_handler("obj_editor_release");
    }

  private:
    Window& window_;
    std::unordered_set<GameObject*> shown_;
    builder::EditableGame& game_;

    bool drag_n_drop_ = false;
    GameObject* drag_target_ = nullptr;
};
}  // namespace ui

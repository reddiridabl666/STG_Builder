#pragma once

#include <string>

#include "EditableGame.hpp"
#include "GameObject.hpp"
#include "Json.hpp"
#include "ObjectEntry.hpp"
#include "UiElement.hpp"

namespace ui {
class ObjectEditor : public Element {
  public:
    ObjectEditor(Window& window, builder::EditableGame& game, nl::json& level_data, nl::json& game_data,
                 const nl::json& entities);

    void draw(const Window&) override;

    ~ObjectEditor() override;

  private:
    nl::json& json_by_obj(const GameObject& obj);
    void erase_obj(const GameObject& obj);
    void update_dragged_obj_pos();

    Window& window_;
    std::unordered_map<std::shared_ptr<GameObject>, std::unique_ptr<ObjectEntry>> shown_;

    nl::json& level_data_;
    nl::json& game_data_;
    builder::EditableGame& game_;

    bool drag_n_drop_ = false;
    sf::Vector2f drag_pos_;
    std::shared_ptr<GameObject> drag_target_;

    std::string obj_types_;
};
}  // namespace ui

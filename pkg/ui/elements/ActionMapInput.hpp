#pragma once

#include "ActionInput.hpp"
#include "Map.hpp"

namespace ui {
class ActionMapInput {
  public:
    void draw(const char* msg) {
        if (ImGui::CollapsingHeader(msg)) {
            draw_map(actions, new_type_);

            std::erase_if(actions, [](const auto& el) {
                return el.second.actions.size() == 0;
            });
        }
    }

  private:
    std::unordered_map<std::string, ActionInput> actions;
    std::string new_type_;

    friend void from_json(const nl::json& json, ActionMapInput& map) {
        map.actions = json.get<decltype(map.actions)>();
    }

    friend void to_json(nl::json& json, const ActionMapInput& map) {
        json = map.actions;
    }
};
}  // namespace ui

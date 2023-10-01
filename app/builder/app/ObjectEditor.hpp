#pragma once

#include <string>
#include <unordered_set>

#include "EditableGame.hpp"
#include "FuncInfo.hpp"
#include "GameObject.hpp"
#include "Json.hpp"
#include "UiElement.hpp"
#include "ui/elements/Stats.hpp"
#include "ui/elements/StringPoint.hpp"

namespace ui {
class ObjectEditor : public Element {
  public:
    ObjectEditor(Window& window, builder::EditableGame& game, nl::json& data, const nl::json& entities);

    void draw(const Window&) override;

    ~ObjectEditor() override;

  private:
    struct ObjectEntry {
        const static inline std::string kDefaultActivityStart =
            std::to_string(GameObject::kDefaultActivityStart);

        const static inline std::unordered_set<std::string> kExcluded = {
            "type", "", "activity_start", "move", "lives", "pos", "rotation"};

        ObjectEntry() : stats(kExcluded) {}

        static ObjectEntry from_json(const nl::json&);
        nl::json to_json() const;

        std::string type = "";
        int type_id = 0;
        std::string activity_start = kDefaultActivityStart;
        StringPoint pos = {};
        int rotation = 0;

        FuncInfo move;
        FuncInfo lives;

        Stats stats;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ObjectEntry, type, activity_start, pos, rotation, move,
                                                    lives)
    };

    Window& window_;
    std::unordered_map<GameObject*, ObjectEntry> shown_;
    nl::json& data_;
    builder::EditableGame& game_;

    bool drag_n_drop_ = false;
    GameObject* drag_target_ = nullptr;
    std::string obj_types_;
};
}  // namespace ui

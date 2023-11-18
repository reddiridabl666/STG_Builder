#pragma once

#include <string>
#include <vector>

#include "Json.hpp"

namespace ui {

struct SingleActionInput {
    struct Value {
        enum class Type {
            Const,
            Property,
        };

        Type type;

        float value;
        std::string property = "score";

        void draw();

        std::unordered_map<std::string, float> with;
        std::string new_deco_;

        friend void from_json(const nl::json&, Value&);
        friend void to_json(nl::json&, const Value&);
    };

    void draw();

    std::string property = "score";
    std::string action = "add";
    Value value;

    int action_id_;

    std::unordered_map<std::string, float> with;
    std::string new_deco_;

    friend void from_json(const nl::json& json, SingleActionInput& input);
    friend void to_json(nl::json& json, const SingleActionInput& input);
};

struct ActionInput {
  public:
    ActionInput();

    ActionInput(const ActionInput&) = default;
    ActionInput& operator=(const ActionInput&) = default;

    ActionInput(ActionInput&&) = default;
    ActionInput& operator=(ActionInput&&) = default;

    void draw();

  private:
    std::vector<SingleActionInput> actions;
    std::unordered_map<std::string, float> with;

    int to_delete_ = -1;

    friend class ActionMapInput;

    friend void from_json(const nl::json& json, ActionInput& map);
    friend void to_json(nl::json& json, const ActionInput& map);
};
}  // namespace ui

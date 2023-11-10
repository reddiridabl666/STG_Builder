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

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SingleActionInput, property, action, value, with)
};

struct ActionInput {
  public:
    void draw();

  private:
    std::vector<SingleActionInput> actions;
    std::unordered_map<std::string, float> with;

    int to_delete_ = -1;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ActionInput, actions, with)
};
}  // namespace ui

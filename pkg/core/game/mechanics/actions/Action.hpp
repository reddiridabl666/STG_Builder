#pragma once

class GameObject;

#include <unordered_map>

#include "Json.hpp"
#include "Value.hpp"

namespace action {
struct Action {
    virtual void operator()(GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~Action() = default;
};

struct BinaryAction : Action {
    virtual void operator()(const GameObject&, GameObject&) const = 0;

    void operator()(GameObject& obj) const override {
        return operator()(obj, obj);
    }
};
}  // namespace action

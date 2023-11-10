#pragma once

class GameObject;

#include <unordered_map>

#include "Json.hpp"
#include "Value.hpp"

namespace action {
struct Action {
    virtual void operator()(std::weak_ptr<GameObject>) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~Action() = default;
};

struct BinaryAction : Action {
    virtual void operator()(std::weak_ptr<const GameObject>, std::weak_ptr<GameObject>) const = 0;

    void operator()(std::weak_ptr<GameObject> obj) const override {
        if (obj.expired()) {
            return;
        }
        return operator()(obj, obj);
    }
};
}  // namespace action

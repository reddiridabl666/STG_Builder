#pragma once

class GameObject;

#include "Json.hpp"
#include "Value.hpp"

namespace action {
struct Action {
    virtual void operator()(const GameObject&, GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~Action() = default;
};
}  // namespace action

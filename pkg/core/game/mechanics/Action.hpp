#pragma once

class GameObject;

#include "Json.hpp"

namespace action {
struct Action {
    virtual void operator()(GameObject&, GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~Action() = default;
};

struct PropertyGetter {
    virtual Value operator()(GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~PropertyGetter() = default;
};
}  // namespace action

#pragma once

#include "Value.hpp"

class GameObject;

namespace action {
struct PropertyGetter {
    virtual Value operator()(GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~PropertyGetter() = default;
};
}  // namespace action

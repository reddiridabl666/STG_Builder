#pragma once

#include "Value.hpp"

class GameObject;

namespace action {
struct Getter {
    virtual Value get(GameObject&) const = 0;
    virtual nl::json to_json() const = 0;
    virtual ~Getter() = default;
};

struct MutableGetter : public Getter {
    virtual Value& get(GameObject&) = 0;

    Value get(GameObject& obj) const override {
        return const_cast<MutableGetter*>(this)->get(obj);
    }
};
}  // namespace action

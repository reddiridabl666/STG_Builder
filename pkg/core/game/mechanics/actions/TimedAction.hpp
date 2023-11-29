#pragma once

#include <memory>

#include "Action.hpp"
#include "Json.hpp"

namespace action {
struct Timed {
    void operator()(float delta_time, std::weak_ptr<GameObject> obj) {
        if (!action || obj.expired()) {
            return;
        }

        cur -= delta_time;
        if (cur > 0) {
            return;
        }

        action->operator()(obj);
        cur = timeout;
    }

  private:
    std::unique_ptr<Action> action;
    float timeout;
    float cur;

    friend void from_json(const nl::json& json, Timed&);
};
}  // namespace action

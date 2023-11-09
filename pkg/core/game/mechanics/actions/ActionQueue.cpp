#include "ActionQueue.hpp"

#include <fmt/core.h>

#include <algorithm>

void action::Queue::push(const std::function<void()>& action, float time) {
    actions_.emplace_back(time, action);
    std::push_heap(actions_.begin(), actions_.end(), std::greater<Entry>{});
}

void action::Queue::resolve(float delta_time) {
    for (auto& action : actions_) {
        action.time -= delta_time;
    }

    std::make_heap(actions_.begin(), actions_.end(), std::greater<Entry>{});
    while (!actions_.empty() && actions_.front().time <= 1e-6) {
        std::pop_heap(actions_.begin(), actions_.end(), std::greater<Entry>{});
        actions_.back().func();
        actions_.pop_back();
    }
}

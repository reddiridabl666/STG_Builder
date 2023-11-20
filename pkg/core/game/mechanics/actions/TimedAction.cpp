#include "TimedAction.hpp"

#include "ActionFactory.hpp"

namespace action {
void from_json(const nl::json& json, Timed& timed) {
    timed.action = Factory::create<Action>(json);
    timed.timeout = json.value("timeout", 2.f);
    timed.cur = json.value("delay", timed.timeout);
}
}  // namespace action

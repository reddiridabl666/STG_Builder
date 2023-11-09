#include "Decorators.hpp"

namespace action {
Value MultiplyGetterDecorator::get(const GameObject& obj) const {
    auto val = original_->get(obj);
    val.mul(multiplier_);
    return val;
}

nl::json MultiplyGetterDecorator::to_json() const {
    auto json = original_->to_json();
    if (json.is_object()) {
        json["/with/mul"] = multiplier_;
        return json;
    }

    return {
        {"property", json},
        {"with", {"mul", multiplier_}},
    };
}
}  // namespace action

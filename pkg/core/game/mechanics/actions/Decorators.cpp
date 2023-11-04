#include "Decorators.hpp"

namespace action {
void TimeoutDecorator::operator()(GameObject& subj, GameObject& obj) const {
    if (current_ >= timeout_) {
        original_->operator()(subj, obj);
        current_ = 0;
        clock_.restart();
        return;
    }

    current_ += clock_.restart().asSeconds();
}

nl::json TimeoutDecorator::to_json() const {
    auto json = original_->to_json();
    json["/with/timeout"_json_pointer] = timeout_;
    return json;
}

Value MultiplyGetterDecorator::operator()(GameObject& obj) const {
    auto val = original_->operator()(obj);
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

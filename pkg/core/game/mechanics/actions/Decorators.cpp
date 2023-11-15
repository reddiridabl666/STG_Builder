#include "Decorators.hpp"

namespace action {
Value MultiplyGetterDecorator::get(const GameObject& obj) const {
    auto val = original_->get(obj);
    val.mul(multiplier_);
    return val;
}

}  // namespace action

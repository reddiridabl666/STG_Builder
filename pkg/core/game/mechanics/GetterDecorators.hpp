#pragma once

#include "Action.hpp"

namespace action {
struct Decorator : public PropertyGetter {
  public:
    Decorator(std::unique_ptr<PropertyGetter>&& getter) : get_(std::move(getter)) {}

  protected:
    std::unique_ptr<PropertyGetter> get_;
};

struct MultiplyDecorator : public Decorator {
  public:
    MultiplyDecorator(std::unique_ptr<PropertyGetter>&& getter, float multiplier)
        : Decorator(std::move(getter)), multiplier_(multiplier) {}

    Value operator()(GameObject& obj) const override {
        auto val = get_->operator()(obj);
        val.mul(multiplier_);
        return val;
    }

  private:
    float multiplier_;
}
}  // namespace action

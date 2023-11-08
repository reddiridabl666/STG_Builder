#pragma once

#include <SFML/System/Clock.hpp>

#include "Action.hpp"
#include "Getter.hpp"

namespace action {

template <typename T>
struct Decorator : public T {
  public:
    Decorator(std::unique_ptr<T>&& original) : original_(std::move(original)) {}

  protected:
    std::unique_ptr<T> original_;
};

struct MultiplyGetterDecorator : public Decorator<Getter> {
  public:
    MultiplyGetterDecorator(std::unique_ptr<Getter>&& getter, float multiplier)
        : Decorator<Getter>(std::move(getter)), multiplier_(multiplier) {}

    Value get(const GameObject& obj) const override;

    nl::json to_json() const override;

  private:
    float multiplier_;
};

struct TimeoutDecorator : public Decorator<Action> {
  public:
    TimeoutDecorator(std::unique_ptr<Action>&& action, float timeout = 1)
        : Decorator<Action>(std::move(action)), timeout_(timeout), current_(timeout) {}

    void operator()(const GameObject&, GameObject&) const override;

    nl::json to_json() const override;

  private:
    float timeout_;

    mutable float current_ = 0;
    mutable sf::Clock clock_;
};
}  // namespace action

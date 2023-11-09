#pragma once

#include <SFML/System/Clock.hpp>

#include "Action.hpp"
#include "ActionQueue.hpp"
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

template <typename ActionType>
struct ActionDecorator : public Decorator<ActionType> {
    using Decorator<ActionType>::Decorator;

    void operator()(const GameObject& subject, GameObject& object) const
        requires std::is_same_v<ActionType, BinaryAction>
    {
        decorator([this, &subject, &object] {
            this->original_->operator()(subject, object);
        });
    }

    void operator()(GameObject& object) const {
        decorator([this, &object] {
            this->original_->operator()(object);
        });
    }

  protected:
    virtual void decorator(const std::function<void()>& fn) const = 0;
};

template <typename ActionType>
struct TimeoutDecorator : public ActionDecorator<ActionType> {
  public:
    TimeoutDecorator(std::unique_ptr<ActionType>&& action, float timeout = 1)
        : ActionDecorator<ActionType>(std::move(action)), timeout_(timeout), current_(timeout) {}

    nl::json to_json() const override;

    void decorator(const std::function<void()>& fn) const override;

  private:
    float timeout_;

    mutable float current_ = 0;
    mutable sf::Clock clock_;
};

template <typename ActionType>
inline nl::json TimeoutDecorator<ActionType>::to_json() const {
    auto json = this->original_->to_json();
    json["/with/timeout"_json_pointer] = timeout_;
    return json;
}

template <typename ActionType>
inline void TimeoutDecorator<ActionType>::decorator(const std::function<void()>& fn) const {
    if (current_ >= timeout_) {
        fn();
        current_ = 0;
        clock_.restart();
        return;
    }

    current_ += clock_.restart().asSeconds();
}

template <typename ActionType>
struct DelayDecorator : public ActionDecorator<ActionType> {
  public:
    DelayDecorator(std::unique_ptr<ActionType>&& action, float delay = 1)
        : ActionDecorator<ActionType>(std::move(action)), delay_(delay) {}

    nl::json to_json() const override;

  protected:
    void decorator(const std::function<void()>& fn) const override;

  private:
    float delay_;
};

template <typename ActionType>
inline void DelayDecorator<ActionType>::decorator(const std::function<void()>& fn) const {
    Queue::get().push(fn, delay_);
}

template <typename ActionType>
inline nl::json DelayDecorator<ActionType>::to_json() const {
    auto json = this->original_->to_json();
    json["/with/delay"_json_pointer] = delay_;
    return json;
}
}  // namespace action

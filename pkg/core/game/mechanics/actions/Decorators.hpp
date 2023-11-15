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

  private:
    float multiplier_;
};

template <typename ActionType>
struct ActionDecorator : public Decorator<ActionType> {
    using Decorator<ActionType>::Decorator;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const
        requires std::is_same_v<ActionType, BinaryAction>
    {
        decorator([this, subject, object] {
            this->original_->operator()(subject, object);
        });
    }

    void operator()(std::weak_ptr<GameObject> object) const override {
        decorator([this, object] {
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

    void decorator(const std::function<void()>& fn) const override;

  private:
    float timeout_;

    mutable float current_ = 0;
    mutable sf::Clock clock_;
};

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
struct DelayDecorator : public Decorator<ActionType> {
  public:
    DelayDecorator(std::unique_ptr<ActionType>&& action, float delay = 1)
        : Decorator<ActionType>(std::move(action)), delay_(delay) {}

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const
        requires std::is_same_v<ActionType, BinaryAction>
    {
        Queue::get().push(
            [this, subject, object] {
                if (subject.expired() || object.expired()) {
                    return;
                }
                this->original_->operator()(subject, object);
            },
            delay_);
    }

    void operator()(std::weak_ptr<GameObject> object) const override {
        Queue::get().push(
            [this, object] {
                if (object.expired()) {
                    return;
                }
                this->original_->operator()(object);
            },
            delay_);
    }

  private:
    float delay_;
};

}  // namespace action

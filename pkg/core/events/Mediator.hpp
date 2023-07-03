#pragma once

#include <functional>

#include "Event.hpp"

class Mediator {
  public:
    using Callback = std::function<void(Event)>;

    static Mediator& get();

    void on(Event::Type event, const Callback& cb);
    void off(Event::Type event, const Callback& cb);
    void emit(Event event);

    Mediator(const Mediator&) = delete;
    Mediator(Mediator&&) = delete;

    Mediator& operator=(const Mediator&) = delete;
    Mediator& operator=(Mediator&&) = delete;

  private:
    Mediator() = default;
    std::array<std::vector<Callback>, Event::TypeCount> cbs_;
};

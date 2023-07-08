#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Event.hpp"

class Observable {
  public:
    using Callback = std::function<void(Event)>;

    void on(Event::Type event, const Callback& cb);
    void emit(Event event);

  private:
    std::unordered_map<Event::Type, std::vector<Callback>> cbs_;
};

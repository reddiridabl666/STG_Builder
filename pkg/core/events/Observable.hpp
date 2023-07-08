#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Event.hpp"

template <EventType T>
class Observable {
  public:
    using Callback = std::function<void(T)>;

    void on(T::Type event, const Callback& cb);
    void emit(T event);

  private:
    std::unordered_map<Event::Type, std::vector<Callback>> cbs_;
};

template <EventType T>
void Observable<T>::on(T::Type event_type, const Callback& cb) {
    cbs_[event_type].push_back(cb);
}

template <EventType T>
void Observable<T>::emit(T event) {
    for (const auto& cb : cbs_[event.type]) {
        cb(event);
    }
}

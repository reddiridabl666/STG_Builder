#pragma once

#include <functional>
#include <unordered_map>

template <typename EventType, typename... Payload>
class Observable {
  public:
    using Callback = std::function<void(const Payload&...)>;

    void on(const EventType& event, const Callback& cb);
    void off(const EventType& event);
    void emit(const EventType& event, const Payload&... payload);

  private:
    std::unordered_multimap<EventType, Callback> cbs_;
};

template <typename EventType, typename... Payload>
void Observable<EventType, Payload...>::on(const EventType& event, const Callback& cb) {
    cbs_.emplace(event, cb);
}

template <typename EventType, typename... Payload>
void Observable<EventType, Payload...>::off(const EventType& event) {
    cbs_.erase(event);
}

template <typename EventType, typename... Payload>
void Observable<EventType, Payload...>::emit(const EventType& event, const Payload&... payload) {
    auto [begin, end] = cbs_.equal_range(event);

    for (auto it = begin; it != end; ++it) {
        it->second(payload...);
    }
}

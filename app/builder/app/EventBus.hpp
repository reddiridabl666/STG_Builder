#pragma once

#include <string>
#include <variant>

#include "Observable.hpp"

class EventBus {
  public:
    enum class Event {
    };

    using payload_type = std::variant<std::string, float>;

    void init(void* object) {
        cbs_.emplace(object, value_type{});
    }

    template <typename Cb>
    void on(void* object, const Event& event, Cb&& cb) {
        cbs_.at(object).on(event, std::forward<Cb>(cb));
    }

    void off(void* object) {
        cbs_.erase(object);
    }

    void emit(const Event& event, const payload_type& payload) {
        for (auto& [_, obs] : cbs_) {
            obs.emit(event, payload);
        }
    }

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    static EventBus& get() {
        static EventBus instance;
        return instance;
    }

  private:
    EventBus() = default;

    using value_type = Observable<Event, std::variant<std::string, float>>;

    std::unordered_map<void*, value_type> cbs_;
};

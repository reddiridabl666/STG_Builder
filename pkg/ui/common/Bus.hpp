#pragma once

#include <functional>
#include <unordered_map>

#include "Json.hpp"

namespace ui {
class Bus {
  public:
    using Cb = std::function<void(const nl::json&)>;

    enum class Event {
        ObjectTypesChanged,
        ObjectTypeChanged,
        ObjectCreated,
        ObjectTypeCreated,
        ObjectDeleted,
    };

    void on(Event event, const std::string& key, const Cb& cb) {
        cbs_[event][key] = cb;
    }

    void off(Event event, const std::string& key) {
        cbs_[event].erase(key);
    }

    void emit(Event event, const nl::json& payload) {
        for (const auto& [_, cb] : cbs_[event]) {
            cb(payload);
        }
    }

    static Bus& get() {
        static Bus bus;
        return bus;
    }

    Bus(const Bus&) = delete;
    Bus& operator=(const Bus&) = delete;
    Bus(Bus&&) = delete;
    Bus& operator=(Bus&&) = delete;

  private:
    Bus() = default;
    std::unordered_map<Event, std::unordered_map<std::string, Cb>> cbs_;
};
}  // namespace ui

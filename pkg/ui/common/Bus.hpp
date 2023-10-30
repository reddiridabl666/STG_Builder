#pragma once

#include <functional>
#include <unordered_map>

#include "Json.hpp"

namespace ui {

enum class Event {
    ObjectTypesChanged,
    ObjectTypeChanged,
    ObjectCreated,
    PlayerCreated,
    ObjectTypeCreated,
    ObjectDeleted,
    GameBgChanged,
    LevelBgChanged,
    SideMenuChanged,
};

template <typename Payload>
class Bus {
  public:
    using Cb = std::function<void(const Payload&)>;

    void on(Event event, const std::string& key, const Cb& cb) {
        cbs_[event][key] = cb;
    }

    void off(Event event, const std::string& key) {
        cbs_[event].erase(key);
    }

    void emit(Event event, const Payload& payload) {
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

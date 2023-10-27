// #pragma once

// #include <SFML/System/Vector2.hpp>
// #include <functional>
// #include <string>
// #include <unordered_map>

// namespace engine {
// class EventBus {
//   public:
//     enum EventType {
//         PosChanged,
//         ValueChanged,
//         Count
//     };

//     struct Event {
//         EventType type;

//         union {
//             sf::Vector2f pos;
//             float val;
//         };
//     };

//     void init(void* object) {
//         cbs_.emplace(object, value_type{});
//     }

//     template <typename Cb>
//     void on(void* object, const Event& event, Cb&& cb) {
//         cbs_.at(object).on(event, std::forward<Cb>(cb));
//     }

//     void off(void* object) {
//         cbs_.erase(object);
//     }

//     void emit(const Event& event, const payload_type& payload) {
//         for (auto& [_, obs] : cbs_) {
//             obs.emit(event, payload);
//         }
//     }

//     EventBus(const EventBus&) = delete;
//     EventBus& operator=(const EventBus&) = delete;

//     static EventBus& get() {
//         static EventBus instance;
//         return instance;
//     }

//   private:
//     EventBus() = default;

//     std::array<std::unordered_map<std::string, std::function<void(const Event&)>>, EventType::Count>
// };
// }  // namespace engine

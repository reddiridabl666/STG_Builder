#pragma once

#include <functional>

namespace action {
class Queue {
  public:
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    void resolve(float delta_time);

    void push(const std::function<void()>& action, float time = 0);

    static Queue& get() {
        static Queue queue;
        return queue;
    }

  private:
    Queue() = default;

    struct Entry {
        float time;
        std::function<void()> func;

        auto operator<=>(const Entry& other) const {
            return time <=> other.time;
        }
    };

    std::vector<Entry> actions_;
};
}  // namespace action

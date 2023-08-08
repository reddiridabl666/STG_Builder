#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

template <typename T, typename P>
class Observable {
  public:
    using Callback = std::function<void(const P&)>;

    void on(const T& event, const Callback& cb);
    void off(const T& event);
    void emit(const T& type, const P& payload);

  private:
    std::unordered_map<T, std::vector<Callback>> cbs_;
};

template <typename T, typename P>
void Observable<T, P>::on(const T& event_type, const Callback& cb) {
    cbs_[event_type].push_back(cb);
}

template <typename T, typename P>
void Observable<T, P>::off(const T& event_type) {
    cbs_.erase(event_type);
}

template <typename T, typename P>
void Observable<T, P>::emit(const T& type, const P& payload) {
    for (const auto& cb : cbs_[type]) {
        cb(payload);
    }
}

#pragma once

#include <functional>
#include <unordered_map>

template <typename T, typename P>
class Observable {
  public:
    using Callback = std::function<void(const P&)>;

    void on(const T& event, const Callback& cb);
    void off(const T& event);
    void emit(const T& event, const P& payload);

  private:
    std::unordered_multimap<T, Callback> cbs_;
};

template <typename T, typename P>
void Observable<T, P>::on(const T& event, const Callback& cb) {
    cbs_.emplace(event, cb);
}

template <typename T, typename P>
void Observable<T, P>::off(const T& event) {
    cbs_.erase(event);
}

template <typename T, typename P>
void Observable<T, P>::emit(const T& event, const P& payload) {
    auto [begin, end] = cbs_.equal_range(event);

    for (auto it = begin; it != end; ++it) {
        it->second(payload);
    }
}

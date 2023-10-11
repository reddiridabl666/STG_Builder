#pragma once

namespace builder {
template <typename State>
struct StateManager {
  public:
    using Cb = std::function<void(State)>;

    StateManager(const Cb& on_state_start, const Cb& on_state_end)
        : on_state_start_(on_state_start), on_state_end_(on_state_end) {}

    void schedule_state_change(State state);
    void resolve_state_change();

    State state() const;

  private:
    void set_state(State state);
    void set_prev_state();

    std::stack<State> states_;
    State next_state_ = State::Undefined;

    Cb on_state_start_;
    Cb on_state_end_;
};

template <typename State>
void StateManager<State>::set_state(State state) {
    if (!states_.empty()) {
        on_state_end_(states_.top());
    }

    on_state_start_(state);
    states_.push(state);
}

template <typename State>
void StateManager<State>::set_prev_state() {
    if (states_.size() <= 1) {
        return;
    }

    on_state_end_(states_.top());
    states_.pop();
    on_state_start_(states_.top());
}

template <typename State>
State StateManager<State>::state() const {
    return states_.top();
}

template <typename State>
void StateManager<State>::schedule_state_change(State state) {
    next_state_ = state;
}

template <typename State>
void StateManager<State>::resolve_state_change() {
    if (next_state_ == State::Undefined) {
        return;
    }

    if (next_state_ == State::Back) {
        set_prev_state();
    } else {
        set_state(next_state_);
    }

    next_state_ = State::Undefined;
}
}  // namespace builder

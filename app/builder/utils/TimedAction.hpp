#pragma once

#include <SFML/System/Clock.hpp>
#include <functional>
#include <thread>

struct TimedAction {
  public:
    template <typename Cb>
    TimedAction(Cb&& cb, size_t seconds) : cb_(std::forward<Cb>(cb)), seconds_(seconds) {}

    void stop() {
        is_running_ = false;
    }

    virtual void run() {
        is_running_ = true;
        clock_.restart();
    }

    void action() {
        if (is_running_ && clock_.getElapsedTime().asSeconds() > seconds_) {
            cb_();
            clock_.restart();
        }
    }

    virtual ~TimedAction() = default;

  protected:
    sf::Clock clock_;
    std::atomic<bool> is_running_ = false;
    std::function<void()> cb_;
    float seconds_;
};

struct AsyncAction : public TimedAction {
  public:
    void run() override {
        TimedAction::run();

        std::thread([this] {
            while (is_running_) {
                if (clock_.getElapsedTime().asSeconds() > seconds_) {
                    cb_();
                    clock_.restart();
                }
            }
        });
    }

    virtual ~AsyncAction() {
        stop();
        thread_.join();
    }

  private:
    std::thread thread_;
};

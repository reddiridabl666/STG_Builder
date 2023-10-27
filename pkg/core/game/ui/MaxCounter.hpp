#pragma once

#include "Counter.hpp"

class MaxCounter : public Counter {
  public:
    MaxCounter(float max, std::shared_ptr<sf::Font>&& font, size_t size)
        : Counter(max, std::move(font), size), max_(max) {
        update(max_);
    }

    void update(float val) override {
        set_contents(fmt::format("{}/{}", val, max_));
    }

  private:
    float max_;
};

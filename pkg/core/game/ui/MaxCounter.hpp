#pragma once

#include "Counter.hpp"

class MaxCounter : public Counter {
  public:
    MaxCounter(float max, const std::string& prefix, std::shared_ptr<sf::Font>&& font, size_t size)
        : Counter(max, prefix, std::move(font), size), max_(max) {
        update(max_);
    }

    void update(float val) override {
        set_contents(fmt::format("{} {}/{}", prefix_, val, max_));
    }

  private:
    float max_;
};

#pragma once

#include "Value.hpp"

template <typename T = int>
class Speed {
  public:
    Speed(T value) : speed_(value) {}

    auto& speed() {
        return speed_;
    }

    const auto& speed() const {
        return const_cast<Speed*>(this)->speed();
    }

  private:
    Value<"speed", T> speed_;
};

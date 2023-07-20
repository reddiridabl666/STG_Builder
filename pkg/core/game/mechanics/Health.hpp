#pragma once

#include "Value.hpp"

template <typename T = int>
class Health {
  public:
    Health(T value) : health_(value) {}

    auto& health() {
        return health_;
    }

    const auto& health() const {
        return const_cast<Health*>(this)->health();
    }

  private:
    Value<"health", T> health_;
};

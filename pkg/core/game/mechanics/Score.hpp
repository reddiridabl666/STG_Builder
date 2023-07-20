#pragma once

#include "Value.hpp"

template <typename T = int>
class Score {
  public:
    Score(T value) : score_(value) {}

    auto& score() {
        return score_;
    }

    const auto& score() const {
        return const_cast<Score*>(this)->score();
    }

  private:
    Value<"score", T> score_;
};

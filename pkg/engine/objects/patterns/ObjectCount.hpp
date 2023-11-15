#pragma once

#include "Pattern.hpp"
#include "Random.hpp"

struct ConstCount : Pattern::ObjectCount {
    ConstCount(size_t count) : count_(count) {}

    size_t get() const override {
        return count_;
    }

  private:
    const size_t count_;
};

struct RandomCount : public Pattern::ObjectCount {
  public:
    RandomCount(size_t left, size_t right) : left_(left), right_(right) {}

    size_t get() const override {
        return rng::get_uint(left_, right_);
    }

  private:
    const size_t left_;
    const size_t right_;
};

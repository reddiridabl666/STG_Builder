#pragma once

#include <functional>

class Value {
  public:
    Value(float val = 0) : val_(val) {}

    void set(float val) {
        val_ = val;
    }

    float get() const {
        return val_;
    }

    operator float() const {
        return val_;
    }

    void add(float val) {
        val_ += val;
    }

    void sub(float val) {
        val_ -= val;
    }

    void inc() {
        ++val_;
    }

    void dec() {
        --val_;
    }

    void mul(float val) {
        val_ *= val;
    }

  private:
    float val_;
};

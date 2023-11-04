#pragma once

#include <Json.hpp>
#include <functional>

class Value {
  public:
    Value(float val = 0, float default_val = 0) : val_(val), default_(default_val) {}

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

    void div(float val) {
        if (abs(val) < 1e-6) {
            return;
        }
        val_ /= val;
    }

    void reset() {
        val_ = default_;
    }

    float default_value() const {
        return default_;
    }

    friend void from_json(const nl::json&, Value&);

  private:
    float val_;
    float default_;
};

void to_json(nl::json&, const Value&);

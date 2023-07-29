#pragma once

// #include <algorithm>
#include <cstddef>

// template <size_t N>
// struct StringLiteral {
//     constexpr StringLiteral(const char (&str)[N]) {
//         std::copy_n(str, N, value);
//     }

//     char value[N];
// };

template <typename T = int>
class Value {
  public:
    Value(T val = 0) : val_(val) {}

    void set(T val) {
        val_ = val;
    }

    T get() const {
        return val_;
    }

    void add(T val) {
        val_ += val;
    }

    void sub(T val) {
        val_ -= val;
    }

    void inc() {
        ++val_;
    }

    void dec() {
        --val_;
    }

    void mul(T val) {
        val_ *= val;
    }

  private:
    T val_;
};

#pragma once

#include "StatDisplay.hpp"

template <typename T = int>
class Bar : public StatDisplay {
  public:
    Bar(Value<T>& val) : val_(val) {}

  private:
    Value<T>& val_;
};

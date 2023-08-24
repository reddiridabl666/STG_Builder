#pragma once

#include "FuncInfo.hpp"

class FuncBuilder {
  public:
    template <typename T>
    static T generate(const FuncInfo& info);
};

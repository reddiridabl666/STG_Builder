#pragma once

#include "FuncInfo.hpp"

using MoveInfo = FuncInfo<>;

using LifeInfo = FuncInfo<float>;

class FuncBuilder {
  public:
    template <typename T, typename FuncInfoType>
    static T generate(const FuncInfoType& info);
};

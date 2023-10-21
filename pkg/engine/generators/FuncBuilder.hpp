#pragma once

#include "FuncInfo.hpp"
#include "Movement.hpp"

class FuncBuilder {
  public:
    template <typename T>
    static T generate(const FuncInfo& info);

    static std::unique_ptr<movement::Rule> generate(const movement::MultiInfo& info);
};

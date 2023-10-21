#pragma once

#include "FuncInfo.hpp"
#include "Movement.hpp"

namespace ui {
void AliveFuncInput(FuncInfo& func);
std::vector<size_t>& MoveFuncInput(movement::MultiInfo& func);
}  // namespace ui

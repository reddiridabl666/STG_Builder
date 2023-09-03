#pragma once

#include <imgui.h>

namespace ui {
using VariableText = std::function<const char*()>;

constexpr auto kStaticWindow = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

constexpr auto kStaticNoResizeWindow = kStaticWindow | ImGuiWindowFlags_NoResize;
}  // namespace ui

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <string>
#include <vector>

namespace ImGui {
void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
void EndGroupPanel();

bool Combo(const char* name, int* cur, const std::vector<std::string>& values);

void VecInput(const char* name, const char* x_name, const char* y_name, float* x, float* y);
void SizeInput(const char* name, float* x, float* y);
void PosInput(const char* name, float* x, float* y, const std::string& id);
}  // namespace ImGui

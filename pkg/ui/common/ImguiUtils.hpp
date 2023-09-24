#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace ImGui {
void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
void EndGroupPanel();

void VecInput(const char* name, const char* x_name, const char* y_name, float* x, float* y);
void PosStringInput(const char* name, std::string* x, std::string* y);
void SizeInput(const char* name, float* x, float* y);
void PosInput(const char* name, float* x, float* y);
}  // namespace ImGui

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace ImGui {
void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
void EndGroupPanel();
void InputLeftLabel(const char* label, std::string* input, ImGuiInputFlags flags = 0);
void InputLeftLabelMultiline(const char* label, std::string* input, const ImVec2& size = {},
                             ImGuiInputFlags flags = 0);
}  // namespace ImGui

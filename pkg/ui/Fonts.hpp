#pragma once

#include <imgui.h>

#include <string>

namespace ui {
ImFont* load_font(const std::string& path, float size) {
    ImGuiIO& io = ImGui::GetIO();
    return io.Fonts->AddFontFromFileTTF(path.c_str(), size);
}

void set_font(ImFont* font) {
    ImGui::PushFont(font);
}

void set_font(const std::string& path, float size) {
    set_font(load_font(path, size));
}
}  // namespace ui

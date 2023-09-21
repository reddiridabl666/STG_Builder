#pragma once

#include <imgui-SFML.h>
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

void set_default_font(const std::string& path, float size) {
    auto io = ImGui::GetIO();
    io.Fonts->Clear();

    io.Fonts->AddFontFromFileTTF(path.c_str(), size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    bool ok = ImGui::SFML::UpdateFontTexture();
    if (!ok) {
        throw std::runtime_error("error loading font");
    }
}
}  // namespace ui

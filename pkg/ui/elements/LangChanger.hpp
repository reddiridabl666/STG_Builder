#pragma once

#include <imgui.h>

#include "Common.hpp"
#include "Messages.hpp"

namespace ui {
class LangChanger {
  public:
    static void draw(const ImVec2& pos, const ImVec2& size) {
        ImGui::SetNextWindowPos(pos);
        // ImGui::SetNextWindowSize(size);

        ImGui::Begin("lang_changer", nullptr,
                     kStaticNoResizeWindow | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

        if (ImGui::Button(lang_to_str(), size)) {
            if (Lang::current() == Lang::RU) {
                Lang::set(Lang::EN);
            } else {
                Lang::set(Lang::RU);
            }
        }
        ImGui::End();
    }

  private:
    static const char* lang_to_str() {
        switch (Lang::current()) {
            case Lang::EN:
                return "EN";
            default:
                return "RU";
        }
    }
};
}  // namespace ui

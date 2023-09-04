#include "Menu.hpp"

#include "EditorMenu.hpp"

namespace ui {
void Menu::draw(const Window&) const {
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginTabBar("##")) {
        for (const auto& tab : tabs_) {
            tab.draw();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Menu::Tab::draw() {
    if (ImGui::BeginTabItem("##")) {
        ImGui::EndTabItem();
    }
}
}  // namespace ui

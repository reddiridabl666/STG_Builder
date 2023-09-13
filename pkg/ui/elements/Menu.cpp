#include "Menu.hpp"

namespace ui {
void Menu::draw(const Window& window) {
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginTabBar("##")) {
        for (auto& tab : tabs_) {
            ImGui::PushID(&tab);
            tab.draw(window);
            ImGui::PopID();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Menu::Tab::draw(const Window& window) const {
    if (ImGui::BeginTabItem(name_())) {
        elem_->draw(window);
        ImGui::EndTabItem();
    }
}
}  // namespace ui

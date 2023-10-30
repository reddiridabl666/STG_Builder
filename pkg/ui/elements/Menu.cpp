#include "Menu.hpp"

namespace ui {
void Menu::draw(const Window& window) {
    auto id = name_() + std::string("##Menu");
    ImGui::Begin(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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
    auto id = name_() + std::string("##");

    if (ImGui::BeginTabItem(id.c_str())) {
        elem_->draw(window);
        ImGui::EndTabItem();
    }
}
}  // namespace ui

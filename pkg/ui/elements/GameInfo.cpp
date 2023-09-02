#include "GameInfo.hpp"

#include <algorithm>

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Utils.hpp"

namespace ui {
void GameInfo::draw(const Window&) const {
    ImGui::BeginGroupPanel(name.c_str(), get_size());

    ImGui::Text(description.c_str());
    ImGui::Text("Levels: %s", std::to_string(levels).c_str());
    ImGui::Text("Last updated: %s", time_to_str(last_updated).c_str());

    ImGui::EndGroupPanel();

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (ImGui::IsItemClicked()) {
        cb_();
    }
}
}  // namespace ui

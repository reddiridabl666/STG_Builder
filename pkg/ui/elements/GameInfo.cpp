#include "GameInfo.hpp"

#include <algorithm>

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Utils.hpp"

namespace ui {
void GameInfo::draw(const Window&) {
    ImGui::BeginGroupPanel(name.c_str(), get_size());

    ImGui::Text(description.c_str());
    ImGui::Text("%s: %s", message(Message::Levels), std::to_string(levels).c_str());
    ImGui::Text(message(Message::LastUpdated), time_to_str(last_updated).c_str());

    ImGui::EndGroupPanel();

    check_mouse();
}
}  // namespace ui

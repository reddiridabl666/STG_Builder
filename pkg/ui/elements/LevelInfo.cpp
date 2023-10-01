#include "LevelInfo.hpp"

#include <fmt/core.h>

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Utils.hpp"

namespace ui {
void LevelInfo::draw(const Window&) {
    ImGui::BeginGroupPanel(fmt::format("{} {}", message(Message::Level), num_).c_str(), get_size());

    bg_.draw();
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text(name_.c_str());
    ImGui::Text(message(Message::LastUpdated), time_to_str(last_updated_).c_str());
    ImGui::EndGroup();

    ImGui::EndGroupPanel();

    check_mouse();
}
}  // namespace ui

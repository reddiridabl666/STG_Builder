#include "GameInfo.hpp"

#include <algorithm>

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Utils.hpp"

namespace ui {
GameInfoBox::GameInfoBox(std::vector<GameInfo>&& games, const ImVec2& size, const ImVec2& pos)
    : Element(pos, size), games_(std::move(games)) {}

void GameInfoBox::draw(const Window& window) const {
    ImGui::SetNextWindowPos(get_pos(), 0, ImVec2{0.5, 0.5});
    ImGui::SetNextWindowSizeConstraints(ImVec2{0, 0}, get_size());

    ImGui::Begin(message(Message::Games), nullptr, kStaticWindow | ImGuiWindowFlags_AlwaysAutoResize);

    for (auto& game : games_) {
        ImGui::PushID(&game);
        game.set_size(ImVec2{ImGui::GetContentRegionAvail().x, 0});
        game.draw(window);
        ImGui::PopID();
    }

    ImGui::End();
}

void GameInfo::draw(const Window&) const {
    ImGui::BeginGroupPanel(name.c_str(), get_size());

    ImGui::Text(description.c_str());
    ImGui::Text("Levels: %s", std::to_string(levels).c_str());
    ImGui::Text("Last updated: %s", time_to_str(last_updated).c_str());

    ImGui::EndGroupPanel();
    ImGui::NewLine();
}
}  // namespace ui

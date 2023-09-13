#include "Button.hpp"

#include "Common.hpp"
#include "ImguiUtils.hpp"

namespace ui {

void Button::draw(const Window&) {
    if (!child_) {
        ImGui::SetNextWindowSize(get_size());
        ImGui::SetNextWindowPos(get_pos());
        ImGui::Begin(text_(), nullptr, kBtnFlags | ImGuiWindowFlags_NoBackground);
    }

    ImGui::Button(text_(), get_size());
    check_mouse();

    if (!child_) {
        ImGui::End();
    }
}

void ImageButton::draw(const Window&) {
    if (!child_) {
        ImGui::SetNextWindowSize(get_size());
        ImGui::SetNextWindowPos(get_pos());
        ImGui::Begin(text_(), nullptr, kBtnFlags);
        ImGui::BeginGroup();
    } else {
        ImGui::BeginChild("##", child_ ? get_size() : ImVec2{0, 0}, child_);
    }

    image_.draw();
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2);
    ImGui::Text(text_());

    if (child_) {
        ImGui::EndChild();
    } else {
        ImGui::EndGroup();
    }

    check_mouse();

    if (!child_) {
        ImGui::End();
    }
}
}  // namespace ui

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "Messages.hpp"
#include "Window.hpp"

namespace ui {
void ErrorPopup(Window& window, const char* msg) {
    ImGui::ErrorCheckEndFrameRecover(nullptr);
    window.display();

    window.main_loop([&window, msg, pop_up = false] mutable {
        if (!pop_up) {
            ImGui::OpenPopup(message(Message::UnexpectedError));
            pop_up = true;
        }

        if (ImGui::BeginPopupModal(message(Message::UnexpectedError), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(msg);
            ImGui::NewLine();

            if (ImGui::Button(message(Message::Close))) {
                window.close();
            }

            ImGui::EndPopup();
        }
    });
}
}  // namespace ui

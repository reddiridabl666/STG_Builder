#include "KeyMapInput.hpp"

#include <imgui.h>

#include "Messages.hpp"
#include "Window.hpp"

namespace ui {
namespace {
void action_input(KeyList& keys, Window& window) {
    for (auto it = keys.begin(); it != keys.end();) {
        auto next = std::next(it);

        auto key = sf::keys_to_str(*it);

        // ImGui::BeginGroup();

        ImGui::Text(key.c_str());
        ImGui::SameLine();

        std::string id = message(Message::Delete) + std::string("##") + key;
        if (ImGui::Button(id.c_str())) {
            keys.erase(it);
        }
        // ImGui::EndGroup();

        it = next;
    }

    if (ImGui::Button(message(Message::Add))) {
        keys.push_back(window.await_key_press());
    }
}
}  // namespace

void KeyMapInput::draw(KeyControls& keys, Window& window) {
    ImGui::SeparatorText(message(Message::Keyboard));

    if (ImGui::CollapsingHeader(message(Message::Movement))) {
        if (ImGui::TreeNode(message(Message::Left))) {
            action_input(keys.left, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Right))) {
            action_input(keys.right, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Up))) {
            action_input(keys.up, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Down))) {
            action_input(keys.down, window);
            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNode(message(Message::Shoot))) {
        action_input(keys.shoot, window);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(message(Message::Slow))) {
        action_input(keys.slow, window);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(message(Message::Bomb))) {
        action_input(keys.special, window);
        ImGui::TreePop();
    }
}
}  // namespace ui

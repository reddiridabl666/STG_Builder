#include "KeyMapInput.hpp"

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Window.hpp"

namespace ui {
namespace {
void action_input(KeyList& keys, Window& window) {
    for (auto it = keys.begin(); it != keys.end();) {
        auto next = std::next(it);

        auto key = sf::keys_to_str(*it);

        ImGui::Text(key.c_str());
        ImGui::SameLine();

        std::string id = message(Message::Delete) + std::string("##") + key;
        if (ImGui::Button(id.c_str())) {
            keys.erase(it);
        }

        it = next;
    }

    if (ImGui::Button(message(Message::Add))) {
        keys.push_back(window.await_key_press());
    }
}
}  // namespace

static std::string new_action = "";
static std::string to_delete = "";

void KeyMapInput::draw(KeyControls& keys, Window& window) {
    ImGui::SeparatorText(message(Message::Keyboard));

    if (ImGui::CollapsingHeader(message(Message::Movement))) {
        if (ImGui::TreeNode(message(Message::Left))) {
            action_input(keys.move.left, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Right))) {
            action_input(keys.move.right, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Up))) {
            action_input(keys.move.up, window);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(message(Message::Down))) {
            action_input(keys.move.down, window);
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(message(Message::Other))) {
        for (auto& [action_name, key] : keys.other) {
            ImGui::PushID(&action_name);
            if (ImGui::TreeNode(action_name.c_str())) {
                action_input(key, window);
                if (ImGui::Button(message(Message::Delete))) {
                    to_delete = action_name;
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        ImGui::InputText(message(Message::Name), &new_action);
        ImGui::SameLine();

        auto id = std::string(message(Message::Add)) + "##key_map";
        if (ImGui::Button(id.c_str())) {
            keys.other.emplace(new_action, KeyList(1, sf::Keyboard::Unknown));
            new_action = "";
        }
    }

    if (to_delete != "") {
        keys.other.erase(to_delete);
        to_delete = "";
    }
}
}  // namespace ui

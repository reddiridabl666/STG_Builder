#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ImguiUtils.hpp"
#include "Messages.hpp"

namespace ui {
inline void draw_map(std::unordered_map<std::string, float>& values, const char* msg, std::string& stat_name) {
    ImGui::SeparatorText(msg);

    for (auto it = values.begin(); it != values.end();) {
        auto next = std::next(it);

        ImGui::InputFloat(it->first.c_str(), &it->second);
        ImGui::SameLine();

        auto label = std::string(message(Message::Delete)) + "##Del" + it->first;
        if (ImGui::Button(label.c_str())) {
            values.erase(it);
        }

        it = next;
    }

    ImGui::InputText("##stat_name", &stat_name);
    ImGui::SameLine();
    if (ImGui::Button(message(Message::AddStat))) {
        values.emplace(stat_name, 0);
        stat_name = "";
    }
}

inline void draw_map(std::unordered_map<std::string, int>& values, const char* msg, std::string& stat_name,
                     const std::unordered_set<std::string>& excluded) {
    ImGui::SeparatorText(msg);

    for (auto it = values.begin(); it != values.end();) {
        auto next = std::next(it);

        ImGui::InputInt(it->first.c_str(), &it->second);
        ImGui::SameLine();

        auto label = std::string(message(Message::Delete)) + "##Del" + it->first;
        if (ImGui::Button(label.c_str())) {
            values.erase(it);
        }

        it = next;
    }

    ImGui::InputText("##stat_name", &stat_name);
    ImGui::SameLine();
    if (ImGui::Button(message(Message::AddStat)) && !excluded.contains(stat_name)) {
        values.emplace(stat_name, 0);
        stat_name = "";
    }
}

template <typename T>
inline void draw_map(std::unordered_map<std::string, T>& values, std::string& stat_name) {
    for (auto it = values.begin(); it != values.end();) {
        auto next = std::next(it);

        ImGui::SeparatorText(it->first.c_str());
        it->second.draw();
        ImGui::SameLine();

        auto label = std::string(message(Message::Delete)) + "##Del" + it->first;
        if (ImGui::Button(label.c_str())) {
            values.erase(it);
        }

        it = next;
    }

    ImGui::InputText("##stat_name", &stat_name);
    ImGui::SameLine();
    if (ImGui::Button(message(Message::AddStat))) {
        values.emplace(stat_name, T{});
        stat_name = "";
    }
}
}  // namespace ui

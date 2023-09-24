#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Json.hpp"
#include "Messages.hpp"

struct Stats {
  public:
    Stats() = default;
    Stats(const std::unordered_set<std::string>& excluded) : excluded_(excluded) {}

    std::unordered_map<std::string, float> values;

    void draw() {
        ImGui::SeparatorText(message(Message::Stats));

        for (auto it = values.begin(); it != values.end();) {
            auto next = std::next(it);

            ImGui::InputFloat(it->first.c_str(), &it->second);
            ImGui::SameLine();

            // ImGui::PushID(&it);
            auto label = std::string(message(Message::Delete)) + "##Del" + it->first;
            if (ImGui::Button(label.c_str())) {
                values.erase(it);
            }
            // ImGui::PopID();

            it = next;
        }

        ImGui::InputText("##stat_name", &stat_name_);
        ImGui::SameLine();
        if (ImGui::Button(message(Message::AddStat)) && !excluded_.contains(stat_name_)) {
            values.emplace(stat_name_, 0);
            stat_name_ = "";
        }
    }

    void from_json(const nl::json& json) {
        for (auto& [key, val] : json.items()) {
            if (!excluded_.contains(key) && val.is_number()) {
                values[key] = val.template get<float>();
            }
        }
    }

    void to_json(nl::json& json) const {
        for (const auto& [key, val] : values) {
            json[key] = val;
        }
    }

  private:
    std::unordered_set<std::string> excluded_;
    std::string stat_name_;
};

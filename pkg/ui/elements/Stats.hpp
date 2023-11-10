#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Json.hpp"
#include "Map.hpp"
#include "Messages.hpp"

struct Stats {
  public:
    Stats() = default;
    Stats(const std::unordered_set<std::string>& excluded) : excluded_(excluded) {}

    std::unordered_map<std::string, int> values;

    void draw() {
        ui::draw_map(values, message(Message::Stats), stat_name_, excluded_);
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

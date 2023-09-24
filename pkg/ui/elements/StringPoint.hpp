#pragma once

#include <string>

#include "Json.hpp"
#include "Messages.hpp"
#include "UiElement.hpp"

namespace ui {
struct StringPoint {
    StringPoint() = default;

    void draw(const char* msg) {
        ImGui::SeparatorText(msg);

        const char* selectables_x[2] = {message(Message::Pixels), "%"};
        const char* selectables_y[2] = {message(Message::Pixels), "%"};

        ImGui::InputFloat("x", &x_);
        ImGui::SameLine();
        ImGui::Combo("##x", &x_percent_, selectables_x, 2);

        ImGui::InputFloat("y", &y_);
        ImGui::SameLine();
        ImGui::Combo("##y", &y_percent_, selectables_y, 2);
    }

  private:
    float x_;
    float y_;

    int x_percent_ = 0;
    int y_percent_ = 0;

    friend void from_json(const nl::json&, StringPoint&);
    friend void to_json(nl::json& json, const StringPoint& res);
};

inline float get_coord(const nl::json& json, const std::string& key, int* percent) {
    auto val = json.at(key);
    if (val.is_number()) {
        return val.template get<float>();
    }

    *percent = 1;

    auto str = val.template get<std::string>();
    return std::stof(str.substr(0, str.size() - 1));
}

inline void from_json(const nl::json& json, StringPoint& res) {
    res.x_ = get_coord(json, "x", &res.x_percent_);
    res.y_ = get_coord(json, "y", &res.y_percent_);
}

inline void to_json(nl::json& json, const StringPoint& res) {
    if (!res.x_percent_) {
        json["x"] = res.x_;
    } else {
        json["x"] = std::to_string(res.x_) + "%";
    }

    if (!res.y_percent_) {
        json["y"] = res.y_;
    } else {
        json["y"] = std::to_string(res.y_) + "%";
    }
}
}  // namespace ui

#include "PatternInput.hpp"

#include <fmt/core.h>

#include "Combo.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "Random.hpp"

namespace ui {
void PatternInput::Count::draw() {
    ImGui::InputText(message(Message::Count), &buffer);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        try {
            left = std::stoul(buffer);
            right = -1;
        } catch (...) {
            if (buffer.starts_with("random(")) {
                auto [l, r] = rng::parse(buffer);
                left = l;
                if (r > l) {
                    r = l;
                }
                right = r;
            }
        }
    }
}

void from_json(const nl::json& json, PatternInput::Count& cnt) {
    if (json.is_number()) {
        cnt.left = json.get<size_t>();
        cnt.buffer = std::to_string(cnt.left);
        return;
    }

    if (json.is_string()) {
        cnt.buffer = json.get<std::string>();
        auto [l, r] = rng::parse(cnt.buffer);
        cnt.left = l;
        cnt.right = r;
    }
}

void to_json(nl::json& json, const PatternInput::Count& cnt) {
    if (cnt.right != -1) {
        json = fmt::format("random({}, {})", cnt.left, cnt.right);
        return;
    }

    json = cnt.left;
}

void from_json(const nl::json& json, PatternInput::Func& fn) {
    for (auto& [key, value] : json.items()) {
        if (key == "type" && value.is_string()) {
            fn.type = value.get<std::string>();
            continue;
        }
        fn.args[key] = value;
    }
}

void to_json(nl::json& json, const PatternInput::Func& fn) {
    json["type"] = fn.type;
    for (auto& [key, value] : fn.args) {
        json[key] = value;
    }
}

static const std::vector<std::string> pos_types = {"line", "circle", "spread"};
static const std::unordered_map<std::string, nl::json> pos_args = {
    {"circle",
     {
         {"radius", 50},
         {"offset", sf::Vector2f{}},
         {"start_angle", 0},
         {"angle", 360},
     }},
    {"line",
     {
         {"sibling_offset", sf::Vector2f{25, 0}},
         {"parent_offset", sf::Vector2f{0, -50}},
     }},
    {"spread",
     {
         {"offset", sf::Vector2f{0, 0}},
         {"space", sf::Vector2f{200, 100}},
     }},
};

static const std::vector<std::string> move_types = {"none",   "from_center", "tracking", "targeted",
                                                    "linear", "circular",    "random",   "following_parent"};
static const std::unordered_map<std::string, nl::json> move_args = {
    {"none", nl::json::object()},
    {"tracking",
     {
         {"tag", "enemy"},
     }},
    {"targeted",
     {
         {"tag", "enemy"},
     }},
    {"from_center", nl::json::object()},
    {"linear",
     {
         {"x", 0},
         {"y", 0},
     }},
    {"circular",
     {
         {"center", sf::Vector2f{}},
         {"speed", 0},
     }},
    {"random",
     {
         {"min", sf::Vector2f{-1, -1}},
         {"max", sf::Vector2f{1, 1}},
     }},
    {"following_parent", nl::json::object()},
};

void PatternInput::Func::draw(const std::vector<std::string>& types,
                              const std::unordered_map<std::string, nl::json>& args_by_type) {
    id = combo::get_item_id(types, type);
    if (ImGui::Combo(message(Message::Type), &id, types)) {
        type = types[id];
        args = args_by_type.at(type);
    }

    for (auto& [key, val] : args) {
        std::string field_label = key + "##func" + type;

        if (val.is_number()) {
            float res = val.get<float>();

            ImGui::InputFloat(field_label.c_str(), &res);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                val = res;
            }
            continue;
        }

        if (val.is_string()) {
            std::string buffer = val.get<std::string>();
            ImGui::InputText(field_label.c_str(), &buffer);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                val = buffer;
            }
        }

        if (!val.is_object()) {
            return;
        }

        sf::Vector2f res = val.get<sf::Vector2f>();
        ImGui::PosInput(field_label.c_str(), &res.x, &res.y, type);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            val = res;
        }
    }
}

void PatternInput::draw() {
    ImGui::InputText(message(Message::BulletType), &bullet);

    count.draw();

    if (ImGui::TreeNode(message(Message::Pos))) {
        pos.draw(pos_types, pos_args);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(message(Message::Movement))) {
        move.draw(move_types, move_args);
        ImGui::TreePop();
    }
}
}  // namespace ui

#include "FuncInput.hpp"

#include "Combo.hpp"
#include "ImguiUtils.hpp"
#include "Life.hpp"
#include "Messages.hpp"

namespace ui {
namespace {
int get_type_id(const char* const* types, size_t size, const std::string& to_find) {
    for (size_t i = 0; i < size; ++i) {
        if (types[i] == to_find) {
            return i;
        }
    }
    return 0;
}

void func_input(const char* const* types, const std::unordered_map<std::string, nl::json>& args, size_t size,
                FuncInfo& func) {
    int idx = get_type_id(types, size, func.type);
    auto id = std::string(message(Message::Type)) + "##" + func.type;
    if (ImGui::Combo(id.c_str(), &idx, types, size)) {
        func.type = types[idx];
        func.args = args.at(func.type);
    }

    for (auto& [key, val] : func.args.items()) {
        std::string field_label = key + "##func" + func.type;

        if (val.is_number()) {
            float res = val.get<float>();

            ImGui::InputFloat(field_label.c_str(), &res);
            if (ImGui::IsItemEdited()) {
                val = res;
            }
            continue;
        }

        if (!val.is_object()) {
            continue;
        }

        sf::Vector2f res = val.get<sf::Vector2f>();
        ImGui::PosInput(field_label.c_str(), &res.x, &res.y, func.type);
        if (ImGui::IsItemEdited()) {
            val = res;
        }
    }
}

void func_input(const char* const* types, const std::unordered_map<std::string, nl::json>& args, size_t size,
                TimedFuncInfo& func) {
    func_input(types, args, size, static_cast<FuncInfo&>(func));
    ImGui::InputFloat(message(Message::Time), &func.time);
}
}  // namespace

static constexpr size_t alive_types_num = sizeof(alive::types) / sizeof(alive::types[0]);

void AliveFuncInput(FuncInfo& func) {
    ImGui::SeparatorText(message(Message::IsAlive));
    func_input(alive::types, alive::args, alive_types_num, func);
    ImGui::NewLine();
}

static constexpr size_t movement_types_num = sizeof(movement::types) / sizeof(movement::types[0]);

int MoveFuncInput(movement::MultiInfo& info) {
    int to_delete = -1;

    ImGui::SeparatorText(message(Message::Movement));

    int cur_id = static_cast<int>(info.repeat);
    if (ImGui::Combo(message(Message::Repeat), &cur_id, message(Message::RepeatTypes))) {
        info.repeat = static_cast<movement::Repeat>(cur_id);
    }

    ImGui::NewLine();

    size_t idx = 0;
    for (auto& func : info.rules) {
        ImGui::PushID(&func);
        func_input(movement::types, movement::args, movement_types_num, func);
        ImGui::PopID();

        auto delete_label = fmt::format("{}##{}", message(Message::Delete), idx);
        if (ImGui::Button(delete_label.c_str())) {
            to_delete = idx;
        }
        ++idx;
        ImGui::NewLine();
    }

    if (ImGui::Button(message(Message::Add))) {
        info.rules.push_back(TimedFuncInfo{"none", nl::json::object(), 0});
    }

    return to_delete;
}
}  // namespace ui

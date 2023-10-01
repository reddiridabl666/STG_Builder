#include "FuncInput.hpp"

#include "ImguiUtils.hpp"
#include "Life.hpp"
#include "Messages.hpp"
#include "Movement.hpp"

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

void func_input(const char* msg, const char* const* types,
                const std::unordered_map<std::string, nl::json>& args, size_t size, FuncInfo& func) {
    ImGui::SeparatorText(msg);

    int idx = get_type_id(types, size, func.type);
    auto id = std::string(message(Message::Type)) + "##" + msg;
    if (ImGui::Combo(id.c_str(), &idx, types, size)) {
        func.type = types[idx];
        func.args = args.at(func.type);
        // fmt::println("{}", func.args.dump(4));
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
    ImGui::NewLine();
}
}  // namespace

void AliveFuncInput(FuncInfo& func) {
    func_input(message(Message::IsAlive), alive::types, alive::args, 4, func);
}

void MoveFuncInput(FuncInfo& func) {
    func_input(message(Message::Movement), movement::types, movement::args, 4, func);
}
}  // namespace ui

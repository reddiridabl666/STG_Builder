#pragma once

#include <imgui.h>

#include <string>
#include <tuple>
#include <type_traits>

#include "UiElement.hpp"

namespace ui {
template <typename T>
struct StatLine {
    std::string prefix;
    T value;
};

class StatBox {
  private:
    template <typename T, typename... Ts>
    static void draw_(T&& val, Ts&&... vals) {
        if constexpr (std::is_pointer_v<decltype(val.value)>) {
            ImGui::Text("%s: %s", val.prefix.c_str(), utils::to_string(*val.value).c_str());
        } else {
            ImGui::Text("%s: %s", val.prefix.c_str(), utils::to_string(val.value).c_str());
        }

        if constexpr (sizeof...(vals) > 0) {
            draw_(vals...);
        }
    }

  public:
    template <typename... Ts>
    static void draw(const std::string name, Ts&&... vals) {
        ImGui::Begin(name.c_str());
        draw_(vals...);
        ImGui::End();
    }
};
}  // namespace ui

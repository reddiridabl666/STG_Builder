#pragma once

#include <imgui.h>

#include <functional>

namespace ui {
class Clickable {
  public:
    Clickable() = default;

    template <typename Cb>
    Clickable(Cb&& cb) : cb_(std::forward<Cb>(cb)) {}

    template <typename Cb>
    void set_cb(Cb&& cb) {
        cb_ = std::forward<Cb>(cb);
    }

  protected:
    void check_mouse() const {
        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (cb_ && ImGui::IsItemClicked()) {
            cb_();
        }
    }

  private:
    std::function<void()> cb_;
};
}  // namespace ui

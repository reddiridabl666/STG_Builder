#pragma once

#include <imgui-SFML.h>

#include <SFML/Graphics/Texture.hpp>
#include <functional>
#include <memory>

#include "ImguiUtils.hpp"
#include "UiElement.hpp"

namespace ui {
class ImageButton : public Element {
  public:
    template <typename Cb>
    ImageButton(const std::function<const char*()>& text, std::shared_ptr<sf::Texture>&& texture,
                const ImVec2& btn_size, Cb&& cb, const ImVec2& size = {}, const ImVec2& pos = {})
        : Element(pos, size),
          text_(text),
          btn_size_(btn_size),
          texture_(texture),
          cb_(std::forward<Cb>(cb)) {}

    void draw(const Window&) const override {
        ImGui::BeginChild("", get_size(), true);
        ImGui::Image(*texture_, btn_size_);
        ImGui::SameLine();
        ImGui::Text(text_());
        ImGui::EndChild();

        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (cb_ && ImGui::IsItemClicked()) {
            cb_();
        }
    }

  private:
    std::function<const char*()> text_;

    ImVec2 btn_size_;
    std::shared_ptr<sf::Texture> texture_;
    std::function<void()> cb_;
};
}  // namespace ui

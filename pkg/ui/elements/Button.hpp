#pragma once

#include <imgui-SFML.h>

#include <SFML/Graphics/Texture.hpp>
#include <memory>

#include "Clickable.hpp"
#include "Common.hpp"
#include "ImguiUtils.hpp"
#include "UiElement.hpp"

namespace ui {

class ImageButton : public Element, public Clickable {
  public:
    template <typename Cb>
    ImageButton(const std::function<const char*()>& text, std::shared_ptr<sf::Texture>&& texture,
                const ImVec2& btn_size, Cb&& cb, bool child = false, const ImVec2& size = {},
                const ImVec2& pos = {})
        : Element(pos, size),
          Clickable(std::forward<Cb>(cb)),
          text_(text),
          btn_size_(btn_size),
          texture_(texture),
          child_(child) {}

    void draw(const Window&) const override {
        if (!child_) {
            ImGui::SetNextWindowSize(get_size());
            ImGui::SetNextWindowPos(get_pos());
            ImGui::Begin(
                text_(), nullptr,
                kStaticNoResizeWindow | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::BeginGroup();
        } else {
            ImGui::BeginChild("##", child_ ? get_size() : ImVec2{0, 0}, child_);
        }

        ImGui::Image(*texture_, btn_size_);
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2);
        ImGui::Text(text_());

        if (child_) {
            ImGui::EndChild();
        } else {
            ImGui::EndGroup();
        }

        if (!child_) {
            ImGui::End();
        }
    }

  private:
    std::function<const char*()> text_;

    ImVec2 btn_size_;
    std::shared_ptr<sf::Texture> texture_;
    bool child_;
};
}  // namespace ui

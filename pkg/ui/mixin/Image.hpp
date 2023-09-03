#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/Texture.hpp>
#include <memory>

namespace ui {
class Image {
  public:
    Image(std::shared_ptr<sf::Texture>&& texture, const ImVec2& size)
        : img_size_(size), texture_(std::move(texture)) {}

    void draw() const {
        ImGui::Image(*texture_, img_size_);
    }

  private:
    ImVec2 img_size_;
    std::shared_ptr<sf::Texture> texture_;
};
}  // namespace ui

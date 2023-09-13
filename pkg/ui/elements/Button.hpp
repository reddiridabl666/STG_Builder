#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <memory>

#include "Clickable.hpp"
#include "Common.hpp"
#include "Image.hpp"
#include "UiElement.hpp"

namespace ui {
class Button : public Element, public Clickable {
  public:
    template <typename Cb>
    Button(const VariableText& text, Cb&& cb, bool child = false, const ImVec2& pos = {},
           const ImVec2& size = {})
        : Element(pos, size), Clickable(std::forward<Cb>(cb)), text_(text), child_(child) {}

    void draw(const Window&) override;

  protected:
    static constexpr auto kBtnFlags =
        kStaticNoResizeWindow | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

    VariableText text_;
    bool child_;
};

class ImageButton : public Button {
  public:
    template <typename Cb>
    ImageButton(const VariableText& text, std::shared_ptr<sf::Texture>&& texture, const ImVec2& img_size,
                Cb&& cb, bool child = false, const ImVec2& pos = {}, const ImVec2& size = {})
        : Button(text, std::forward<Cb>(cb), child, pos, size), image_(std::move(texture), img_size) {}

    void draw(const Window&) override;

  private:
    Image image_;
};
}  // namespace ui

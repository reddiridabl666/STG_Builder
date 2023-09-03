#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Clickable.hpp"
#include "Image.hpp"
#include "UiElement.hpp"

namespace ui {
struct LevelInfo : public Element, public Clickable {
  public:
    LevelInfo(std::string name, size_t num, std::shared_ptr<sf::Texture>&& texture, const ImVec2& img_size,
              time_t last_updated = time(nullptr))
        : name_(name), num_(num), last_updated_(last_updated), bg_(std::move(texture), img_size) {}

    void draw(const Window&) const override;

  private:
    std::string name_;
    size_t num_;
    time_t last_updated_;
    Image bg_;
};
}  // namespace ui

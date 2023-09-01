#pragma once

#include <memory>
#include <vector>

#include "UiElement.hpp"

namespace ui {
struct Box : public Element {
  public:
    Box(std::vector<std::unique_ptr<Element>>&& elems, const ImVec2& size, const ImVec2& pos = {});

    void draw(const Window&) const override;

  private:
    mutable std::vector<std::unique_ptr<Element>> elems_;
};
}  // namespace ui

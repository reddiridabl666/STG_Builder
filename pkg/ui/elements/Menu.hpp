#pragma once

#include "UiElement.hpp"

namespace ui {

class Menu : public Element {
  public:
    struct Tab {
        void draw() const;
    };

    void draw(const Window&) const override;

  private:
    std::vector<Tab> tabs_;
};
}  // namespace ui

#pragma once

#include <memory>

#include "UiElement.hpp"

namespace ui {

class Menu : public Element {
  public:
    struct Tab {
        Tab(std::unique_ptr<Element>&& elem, const VariableText& name) : name_(name), elem_(std::move(elem)) {}

        void draw(const Window&) const;

      private:
        VariableText name_;
        std::unique_ptr<Element> elem_;
    };

    Menu(std::vector<Tab>&& tabs, const VariableText& name) : name_(name), tabs_(std::move(tabs)) {}

    void draw(const Window&) override;

  private:
    VariableText name_;
    std::vector<Tab> tabs_;
};
}  // namespace ui

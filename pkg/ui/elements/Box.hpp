#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "UiElement.hpp"

namespace ui {
struct Box : public Element {
  public:
    using Items = std::vector<std::unique_ptr<Element>>;

    Box(const std::function<const char*()>& name, Items&& elems, const ImVec2& size, const ImVec2& pos = {});

    void draw(const Window&) override;

    size_t size() const {
        return elems_.size();
    }

    void push_back(Items::value_type&& elem) {
        elems_.push_back(std::move(elem));
    }

  private:
    std::function<const char*()> name_;
    Items elems_;
};
}  // namespace ui

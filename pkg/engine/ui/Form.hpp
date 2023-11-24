#pragma once

#include "Window.hpp"

template <typename LayoutType>
class Form {
  public:
    Form(std::unique_ptr<Displayable>&& container, float offset) : layout_(std::move(container), offset) {}
    Form(LayoutType&& layout) : layout_(std::move(layout)) {}

    void draw(Window& window) {
        layout_.draw(window);
    }

    void set_pos(const sf::Vector2f& pos) {
        layout_.set_pos(pos);
    }

    sf::Vector2f get_size() const {
        return layout_.get_size();
    }

  protected:
    LayoutType layout_;
};

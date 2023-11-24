#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public VerticalLayout {
  public:
    void add_item(std::unique_ptr<Displayable>&& item) {
        items_.push_back(std::move(item));
    }

    void set_pos(const sf::Vector2f& pos) override;

    void draw(Window& window) const override;

    void reset() {
        set_pos(pos());
    }

  private:
    size_t start_;
    size_t num_shown_;
};

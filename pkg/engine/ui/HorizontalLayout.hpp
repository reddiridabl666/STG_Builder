#pragma once

#include "Layout.hpp"

class HorizontalLayout : public Layout {
  public:
    HorizontalLayout(float offset) : offset_(offset) {}

    HorizontalLayout(std::unique_ptr<Displayable>&& container, std::vector<std::unique_ptr<Displayable>>&& items,
                     float offset);

    void set_pos(const sf::Vector2f& pos) override;

    void init(size_t from = 0);

  private:
    float offset_;
};

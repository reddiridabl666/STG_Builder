#pragma once

#include "Layout.hpp"

class VerticalLayout : public Layout {
  public:
    VerticalLayout(float offset) : offset_(offset) {}

    VerticalLayout(std::unique_ptr<Displayable>&& container, std::vector<std::unique_ptr<Displayable>>&& items,
                   float offset, size_t init = 0);

    template <typename... Args>
    VerticalLayout(std::unique_ptr<Displayable>&& container, float offset, size_t init_value = 0, Args&&... args)
        : Layout(std::move(container), args...), offset_(offset) {
        init(init_value);
    }

    void set_pos(const sf::Vector2f& pos) override;

    void init(size_t from = 0);

  private:
    float offset_;
};

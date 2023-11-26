#pragma once

#include "Layout.hpp"

class VerticalLayout : public Layout {
  public:
    VerticalLayout(float offset = 0) : offset_(offset) {}

    VerticalLayout(std::unique_ptr<Displayable>&& container, std::vector<std::unique_ptr<Displayable>>&& items,
                   float offset, size_t init = 0);

    VerticalLayout(std::unique_ptr<Displayable>&& container, float offset)
        : Layout(std::move(container)), offset_(offset) {}

    template <typename... Items>
    VerticalLayout(std::unique_ptr<Displayable>&& container, float offset, size_t init_val, Items&&... items)
        : Layout(std::move(container), std::forward<Items>(items)...), offset_(offset) {
        init(init_val);
    }

    void set_pos(const sf::Vector2f& pos) override;
    void set_size(const sf::Vector2f& size, bool save_proportions = false) override;

    void init(size_t from = 0);

  protected:
    float offset_;
};

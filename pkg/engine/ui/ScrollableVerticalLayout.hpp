#pragma once

#include "VerticalLayout.hpp"

class ScrollableVerticalLayout : public VerticalLayout {
  public:
    ScrollableVerticalLayout(Window& window, std::unique_ptr<Displayable>&& container,
                             std::vector<std::unique_ptr<Displayable>>&& items, size_t num_shown, float offset_y);

    void set_pos(const sf::Vector2f& pos) override;

    void draw(Window& window) const override;

    void reset() {
        set_pos(pos());
    }

    void scroll(int by = 1);
    void scroll_non_cycle(int by = 1);

    void init();

    ~ScrollableVerticalLayout() override;

  private:
    static inline size_t max_id_ = 0;

    size_t start_ = 0;
    size_t num_shown_;
    Window& window_;
    size_t id_;
};

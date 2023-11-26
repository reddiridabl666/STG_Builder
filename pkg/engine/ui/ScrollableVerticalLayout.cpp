#include "ScrollableVerticalLayout.hpp"

#include <fmt/core.h>

ScrollableVerticalLayout::ScrollableVerticalLayout(Window& window, std::unique_ptr<Displayable>&& container,
                                                   std::vector<std::unique_ptr<Displayable>>&& items, size_t num_shown,
                                                   float offset_y)
    : VerticalLayout(std::move(container), std::move(items), offset_y),
      num_shown_(num_shown),
      window_(window),
      id_(max_id_++) {
    window_.add_handler(
        fmt::format("scroll_layout_{}", id_), sf::Event::MouseWheelScrolled, [this](const sf::Event& event) {
            if (get_bounds().contains(window_.pixel_to_coords(event.mouseWheelScroll.x, event.mouseWheelScroll.y))) {
                scroll(event.mouseWheelScroll.delta > 0 ? 1 : -1);
            }
        });

    init();
}

void ScrollableVerticalLayout::scroll(int by) {
    if (start_ < std::abs(by) && by < 0) {
        start_ = 0;
    } else if (start_ + by >= items_.size() - num_shown_) {
        start_ = items_.size() - num_shown_;
    } else {
        start_ += by;
    }

    reset();
}

void ScrollableVerticalLayout::scroll_non_cycle(int) {}

void ScrollableVerticalLayout::init() {
    normalize_width(0);

    auto widest = std::max_element(items_.begin(), items_.end(), [](const auto& left, const auto& right) {
        return left->width() < right->width();
    });

    float container_height = [this] {
        float res = 0;
        for (size_t i = start_; i < std::min(start_ + num_shown_, items_.size()); ++i) {
            res += items_[i]->height();
        }
        return res;
    }();

    container_height += offset_ * std::min(items_.size(), num_shown_);
    float container_width = std::max(container_->width(), (*widest)->width());

    container_->set_size(sf::Vector2f{container_width, container_height});

    for (auto& item : items_) {
        item->set_origin(item->get_size() / 2);
    }
}

ScrollableVerticalLayout::~ScrollableVerticalLayout() {
    window_.remove_handler(fmt::format("scroll_layout_{}", id_));
}

void ScrollableVerticalLayout::set_pos(const sf::Vector2f& pos) {
    container_->set_pos(pos);
    if (items_.empty()) {
        return;
    }

    items_[start_]->set_pos(container_->pos() - container_->get_origin() +
                            sf::Vector2f{container_->width() / 2, items_[start_]->height() / 2});
    for (size_t i = start_ + 1; i < std::min(start_ + num_shown_, items_.size()); ++i) {
        auto offset = sf::Vector2f{0, offset_ + items_[i - 1]->height() / 2 + items_[i]->height() / 2};
        items_[i]->set_pos(items_[i - 1]->pos() + offset);
    }
}

void ScrollableVerticalLayout::draw(Window& window) const {
    container_->draw(window);
    for (size_t i = start_; i < std::min(start_ + num_shown_, items_.size()); ++i) {
        items_[i]->draw(window);
    }
}

#include "ScrollableVerticalLayout.hpp"

void ScrollableVerticalLayout::set_pos(const sf::Vector2f& pos) {
    container_->set_pos(pos);
    if (items_.empty()) {
        return;
    }

    items_[start_]->set_pos(container_->pos() - container_->get_origin() +
                            sf::Vector2f{container_->width() / 2, offset_ + items_[start_]->height() / 2});
    for (size_t i = start_; i < start_ + num_shown_; ++i) {
        auto offset = sf::Vector2f{0, offset_ + items_[i - 1]->height() / 2 + items_[i]->height() / 2};
        items_[i]->set_pos(items_[i - 1]->pos() + offset);
    }
}

void ScrollableVerticalLayout::draw(Window& window) const {
    container_->draw(window);
    for (size_t i = start_; i < start_ + num_shown_; ++i) {
        items_[start_]->draw(window);
    }
}

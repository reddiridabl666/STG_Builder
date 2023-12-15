#include "VerticalLayout.hpp"

#include <numeric>

VerticalLayout::VerticalLayout(std::unique_ptr<Displayable>&& container,
                               std::vector<std::unique_ptr<Displayable>>&& items, float offset, size_t init_value)
    : Layout(std::move(container), std::move(items)), offset_(offset) {
    init(init_value);
}

void VerticalLayout::set_pos(const sf::Vector2f& pos) {
    container_->set_pos(pos);
    if (items_.empty()) {
        return;
    }

    items_.front()->set_pos(container_->pos() - container_->get_origin() +
                            sf::Vector2f{container_->width() / 2, offset_ + items_.front()->height() / 2});
    for (size_t i = 1; i < items_.size(); ++i) {
        auto offset = sf::Vector2f{0, offset_ + items_[i - 1]->height() / 2 + items_[i]->height() / 2};
        items_[i]->set_pos(items_[i - 1]->pos() + offset);
    }
}

void VerticalLayout::set_size(const sf::Vector2f& size, bool save_proportions) {
    Layout::set_size(size, save_proportions);
    for (auto& item : items_) {
        item->set_width(size.x);
        item->set_origin(item->get_size() / 2);
    }
}

void VerticalLayout::init(size_t from) {
    normalize_width(from);

    auto widest = std::max_element(items_.begin(), items_.end(), [](const auto& left, const auto& right) {
        return left->width() < right->width();
    });

    float container_height = [this] {
        float res = 0;
        for (auto& item : items_) {
            res += item->height();
        }
        return res;
    }();

    container_height += offset_ * (items_.size() + 1);
    float container_width = [&] {
        if (widest == items_.end()) {
            return container_->width();
        }
        return std::max(container_->width(), (*widest)->width() + offset_);
    }();

    container_->set_size(sf::Vector2f{container_width, container_height});

    for (auto& item : items_) {
        item->set_origin(item->get_size() / 2);
    }
}

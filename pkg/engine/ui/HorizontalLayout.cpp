#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(std::unique_ptr<Displayable>&& container,
                                   std::vector<std::unique_ptr<Displayable>>&& items, float offset)
    : Layout(std::move(container), std::move(items)), offset_(offset) {
    init();
}

void HorizontalLayout::set_pos(const sf::Vector2f& pos) {
    container_->set_pos(pos);
    if (items_.empty()) {
        return;
    }

    items_.front()->set_pos(container_->pos() - container_->get_origin() +
                            sf::Vector2f{offset_ + items_.front()->width() / 2, container_->height() / 2});
    for (size_t i = 1; i < items_.size(); ++i) {
        auto offset = sf::Vector2f{offset_ + items_[i - 1]->width() / 2 + items_[i]->width() / 2, 0};
        items_[i]->set_pos(items_[i - 1]->pos() + offset);
    }
}

void HorizontalLayout::init(size_t from) {
    normalize_width(from);

    auto highest = std::max_element(items_.begin(), items_.end(), [](const auto& left, const auto& right) {
        return left->height() < right->height();
    });

    float container_width = [this] {
        float res = 0;
        for (auto& item : items_) {
            res += item->width();
        }
        return res;
    }();

    container_width += offset_ * (items_.size() + 1);
    float container_height = std::max(container_->height(), (*highest)->height());

    container_->set_size(sf::Vector2f{container_width, container_height});

    for (auto& item : items_) {
        item->set_origin(item->get_size() / 2);
    }
}

#include "Layout.hpp"

void Layout::draw(Window& window) const {
    container_->draw(window);
    for (auto& item : items_) {
        item->draw(window);
    }
}

void Layout::scale(float x, float y) {
    container_->scale(x, y);
    for (auto& item : items_) {
        item->scale(x, y);
    }
}

void Layout::set_rotation(float angle) {
    container_->set_rotation(angle);
    for (auto& item : items_) {
        item->set_rotation(angle);
    }
}

float Layout::max_width(const std::vector<std::unique_ptr<Displayable>>& items, size_t from) {
    float max = std::numeric_limits<float>::min();

    for (size_t i = std::min(from, items.size()); i < items.size(); ++i) {
        if (items[i]->width() > max) {
            max = items[i]->width();
        }
    }
    return max;
}

void Layout::normalize_width(size_t from) {
    float width = max_width(items_, from);
    for (size_t i = std::min(from, items_.size()); i < items_.size(); ++i) {
        items_[i]->set_width(width);
    }
}

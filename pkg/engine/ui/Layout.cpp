#include "Layout.hpp"

#include <algorithm>

// void Layout::insert_item(size_t pos, std::unique_ptr<Displayable>&& item, size_t init_val) {
//     if (pos > items_.size()) {
//         return;
//     }

//     items_.insert(items_.begin() + pos, std::move(item));
//     init(init_val);
// }

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

void Layout::normalize_width(size_t from) {
    auto it = std::max_element(items_.begin() + from, items_.end(), [](const auto& left, const auto& right) {
        return left->width() < right->width();
    });

    if (it == items_.end()) {
        return;
    }

    float width = (*it)->width();
    for (size_t i = std::min(from, items_.size()); i < items_.size(); ++i) {
        items_[i]->set_width(width);
    }
}

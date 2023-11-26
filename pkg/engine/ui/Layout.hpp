#pragma once

#include <memory>
#include <vector>

#include "Displayable.hpp"

class Layout : public Displayable {
  public:
    Layout() = default;

    Layout(std::unique_ptr<Displayable>&& container, std::vector<std::unique_ptr<Displayable>>&& items)
        : container_(std::move(container)), items_(std::move(items)) {}

    template <typename... Items>
    Layout(std::unique_ptr<Displayable>&& container, Items&&... items) : container_(std::move(container)) {
        add_items(std::forward<Items>(items)...);
    }

    template <typename... Items>
    void add_items(Items&&... items) {
        items_.reserve(items_.size() + sizeof...(items));
        (items_.push_back(std::move(items)), ...);
    }

    void clear() {
        items_.clear();
    }

    // virtual void init(size_t from) = 0;

    // void insert_item(size_t pos, std::unique_ptr<Displayable>&& item, size_t init_val = 0);

    Layout(Layout&&) = default;
    Layout& operator=(Layout&&) = default;

    void draw(Window& window) const override;

    void set_pos(const sf::Vector2f& pos) = 0;

    sf::Vector2f get_size() const override {
        return container_->get_size();
    }

    sf::FloatRect get_bounds() const override {
        return container_->get_bounds();
    }

    sf::Transformable& transformable() override {
        return container_->transformable();
    }

    void scale(float x, float y) override;

    void set_rotation(float angle) override;

    void set_size(const sf::Vector2f& size, bool save_proportions = false) override;

    void set_container(std::unique_ptr<Displayable>&& container) {
        container_ = std::move(container);
    }

    void set_items(std::vector<std::unique_ptr<Displayable>>&& items) {
        items_ = std::move(items);
    }

  protected:
    void normalize_width(size_t from = 0);

    std::unique_ptr<Displayable> container_;
    std::vector<std::unique_ptr<Displayable>> items_;
};

#pragma once

#include "Displayable.hpp"
#include "LinAlg.hpp"

class DummyDisplayable : public Displayable {
  public:
    void draw(Window&) const {}

    sf::Vector2f get_size() const override {
        return size_;
    }

    sf::FloatRect get_bounds() const override {
        return sf::FloatRect{pos(), get_size()};
    }

    sf::Transformable& transformable() override {
        return dummy_;
    }

    void set_size(const sf::Vector2f& size, bool save_proportions) override {
        size_ = size;
        Displayable::set_size(size, save_proportions);
    }

    void scale(float x, float y) {
        size_.x *= x;
        size_.y *= y;
        Displayable::scale(x, y);
    }

  private:
    sf::Transformable dummy_;
    sf::Vector2f size_;
};

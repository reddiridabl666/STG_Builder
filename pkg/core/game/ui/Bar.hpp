#pragma once

#include "GameUi.hpp"
#include "SpriteObject.hpp"

class Bar : public GameUi {
  public:
    Bar(float max, float width, std::shared_ptr<sf::Texture>&& empty, std::shared_ptr<sf::Texture>&& full)
        : empty_(std::move(empty)), full_(std::move(full)), max_(max) {
        update(max_);
        set_width(width);
    }

    sf::Drawable& drawable() override {
        return empty_.drawable();
    }

    sf::Transformable& transformable() override {
        return empty_.transformable();
    }

    sf::Vector2f get_size() const override {
        return empty_.get_size();
    }

    sf::FloatRect get_bounds() const override {
        return empty_.get_bounds();
    }

    void set_pos(const sf::Vector2f& pos) override {
        empty_.set_pos(pos);
        full_.set_pos(pos);
    }

    void move(const sf::Vector2f& offset) override {
        empty_.move(offset);
        full_.move(offset);
    }

    void rotate(float angle) override {
        empty_.rotate(angle);
        full_.rotate(angle);
    }

    void set_rotation(float angle) override {
        empty_.set_rotation(angle);
        full_.set_rotation(angle);
    }

    void scale(float x, float y) override {
        empty_.scale(x, y);
        full_.scale(x, y);
    }

    void draw(Window& window) const override {
        empty_.draw(window);
        full_.draw(window);
    }

    void update(float val) override {
        auto size = full_.texture_size();
        full_.set_texture_rect(sf::IntRect{
            0,
            0,
            static_cast<int>(val / max_ * size.x),
            static_cast<int>(size.y),
        });
    }

  private:
    SpriteObject empty_;
    SpriteObject full_;

    float max_;
};

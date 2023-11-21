#pragma once

#include "Text.hpp"

class Button : public Displayable {
  public:
    Button(std::unique_ptr<Displayable>&& bg, Text&& text, std::function<void()>&& cb = nullptr, sf::Vector2f pos = {});

    sf::Drawable& drawable() override {
        return bg_->drawable();
    }

    void set_cb(std::function<void()>&& cb) {
        cb_ = std::move(cb);
    }

    sf::Vector2f get_size() const override {
        return bg_->get_size();
    }

    sf::FloatRect get_bounds() const override {
        return bg_->get_bounds();
    }

    sf::Transformable& transformable() override {
        return bg_->transformable();
    }

    void set_pos(const sf::Vector2f& pos) override;

    void set_rotation(float angle) override {
        bg_->set_rotation(angle);
        text_.set_rotation(angle);
    }

    void scale(float x, float y) override {
        bg_->scale(x, y);
        text_.scale(x, y);
    }

    void draw(Window& window) const override {
        window.draw(*bg_);
        window.draw(text_);

        auto mouse_pos = window.pixel_to_coords(sf::Mouse::getPosition(window.base()));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && get_bounds().contains(mouse_pos)) {
            cb_();
        }
    }

  private:
    std::unique_ptr<Displayable> bg_;
    Text text_;
    std::function<void()> cb_;
    float padding_ = 10;
};

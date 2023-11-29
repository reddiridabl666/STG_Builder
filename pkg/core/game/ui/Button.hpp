#pragma once

#include "Text.hpp"

class Button : public Displayable {
  public:
    Button(std::unique_ptr<Displayable>&& bg, Text&& text, std::function<void()>&& cb = nullptr, sf::Vector2f pos = {});

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

    void set_size(const sf::Vector2f& size, bool save_proportions) override {
        bg_->set_size(size, save_proportions);
    }

    void draw(Window& window) const override;

  private:
    std::unique_ptr<Displayable> bg_;
    Text text_;
    std::function<void()> cb_;
    float padding_ = 10;
    mutable bool was_pressed_ = false;
};

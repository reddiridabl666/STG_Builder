#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Text.hpp"

class TextInput : public Displayable {
  public:
    TextInput(Window& window, std::unique_ptr<Displayable>&& bg, Text&& text,
              std::function<void(const std::string&)>&& cb = nullptr, sf::Vector2f pos = {});

    void set_cb(std::function<void(const std::string&)>&& cb) {
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

    ~TextInput() override;

  private:
    std::unique_ptr<Displayable> bg_;
    Text text_;
    mutable bool is_focused_ = false;
    std::string buffer_;

    std::function<void(const std::string&)> cb_;
    Window& window_;

    float padding_ = 10;
};

#pragma once

#include <memory>

#include "Displayable.hpp"

class Text : virtual public Displayable {
  public:
    Text(const std::string& content, std::shared_ptr<sf::Font>&& font, size_t size)
        : font_(std::move(font)), text_(content, *font_, size), rect_(get_size()) {
        rect_.setOutlineColor(sf::Color::Green);
        rect_.setFillColor(sf::Color::Transparent);
        rect_.setOutlineThickness(2);
    }

    sf::Drawable& drawable() override {
        return text_;
    }

    void draw(Window& window) const override {
        rect_.setPosition(text_.getPosition());

        window.draw(text_);
        window.draw(rect_);
    }

    sf::Transformable& transformable() override {
        return text_;
    }

    sf::Vector2f get_size() const override {
        auto bounds = get_bounds();
        return sf::Vector2f{bounds.width, bounds.height};
    }

    sf::FloatRect get_bounds() const override {
        return text_.getGlobalBounds();
    }

    void set_size(size_t size) {
        text_.setCharacterSize(size);
    }

    void set_font(std::shared_ptr<sf::Font>&& font) {
        font_ = std::move(font);
        text_.setFont(*font_);
    }

    void set_contents(const std::string& contents) {
        text_.setString(contents);
    }

  private:
    std::shared_ptr<sf::Font> font_;
    sf::Text text_;
    mutable sf::RectangleShape rect_;
};

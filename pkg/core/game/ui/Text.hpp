#pragma once

#include "Displayable.hpp"

class Text : public Displayable {
  public:
    Text(const std::string& content, std::shared_ptr<sf::Font>&& font, size_t size)
        : font_(std::move(font)), text_(content, *font_, size) {}

    sf::Drawable& drawable() override {
        return text_;
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
};

#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Json.hpp"

struct TextProps {
    std::string text = "";
    std::string font = "";
    size_t font_size = 30;
    sf::Color color = sf::Color::White;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TextProps, text, font, color, font_size);
};

class Text : virtual public Displayable {
  public:
    Text(const std::string& content, std::shared_ptr<sf::Font>&& font, size_t size, sf::Color color = sf::Color::White)
        : font_(std::move(font)), text_(content, *font_, size) {
        text_.setFillColor(color);
    }

    Text(Text&&) = default;
    Text& operator=(Text&&) = default;

    sf::Drawable& drawable() override {
        return text_;
    }

    std::string contents() const {
        return text_.getString();
    }

    sf::Transformable& transformable() override {
        return text_;
    }

    sf::Vector2f get_size() const override {
        auto bounds = text_.getLocalBounds();
        return sf::Vector2f{bounds.width, bounds.height} * text_.getScale();
    }

    sf::FloatRect get_bounds() const override {
        return text_.getGlobalBounds();
    }

    void set_font_size(size_t size) {
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

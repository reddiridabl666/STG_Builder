#include "Button.hpp"

Button::Button(std::unique_ptr<Displayable>&& bg, Text&& text, std::function<void()>&& cb, sf::Vector2f pos)
    : bg_(std::move(bg)), text_(std::move(text)), cb_(std::move(cb)) {
    text_.set_origin(text_.get_size() / 2);

    float x = bg_->get_size().x;
    if (x < text_.get_size().x + padding_ * 2) {
        x = text_.get_size().x + padding_ * 2;
    }

    bg_->set_size(sf::Vector2f{x, text_.get_size().y + padding_ * 2}, false);
    set_pos(pos);
}

void Button::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    text_.set_pos(pos - bg_->get_origin() + bg_->get_size() / 2 - sf::Vector2f{0, 8});
}

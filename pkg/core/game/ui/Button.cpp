#include "Button.hpp"

Button::Button(std::unique_ptr<Displayable>&& bg, Text&& text, std::function<void()>&& cb, sf::Vector2f pos)
    : bg_(std::move(bg)), text_(std::move(text)), cb_(std::move(cb)) {
    text_.set_origin(text_.get_size() / 2);

    float x = bg_->width();
    if (x < text_.width() + padding_ * 2) {
        x = text_.width() + padding_ * 2;
    }

    bg_->set_size(sf::Vector2f{x, text_.get_size().y + padding_ * 2}, false);
    set_pos(pos);
}

void Button::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    text_.set_pos(pos - bg_->get_origin() + bg_->get_size() / 2 - sf::Vector2f{0, 8});
}

void Button::draw(Window& window) const {
    window.draw(*bg_);
    window.draw(text_);

    auto mouse_pos = window.pixel_to_coords(sf::Mouse::getPosition(window.base()));
    if (get_bounds().contains(mouse_pos)) {
        window.set_mouse_cursor(sf::Cursor::Hand);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            cb_();
        }
    }
}

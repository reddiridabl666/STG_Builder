#include "TextInput.hpp"

#include "Controls.hpp"

TextInput::TextInput(Window& window, std::unique_ptr<Displayable>&& bg, Text&& text,
                     std::function<void(const std::string&)>&& cb, sf::Vector2f pos)
    : bg_(std::move(bg)), text_(std::move(text)), cb_(std::move(cb)), window_(window) {
    float x = bg_->width();
    if (x < text_.width() + padding_ * 2) {
        x = text_.width() + padding_ * 2;
    }

    // bg_->set_size(sf::Vector2f{x, text_.height() + padding_ * 2}, false);
    set_pos(pos);

    window.add_handler("text_input_handler", sf::Event::KeyPressed, [this](const sf::Event& event) {
        if (!is_focused_) {
            return;
        }

        if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
            auto key = sf::keys_to_str(event.key.code);
            if (!event.key.shift) {
                key[0] = tolower(key[0]);
            }
            buffer_ += key;
            text_.set_contents(buffer_);
            if (text_.width() > bg_->width()) {
            }
        }

        if (event.key.code == sf::Keyboard::Backspace && !buffer_.empty()) {
            buffer_.pop_back();
            text_.set_contents(buffer_);
        }

        if (event.key.code == sf::Keyboard::Enter && buffer_.size() > 0) {
            is_focused_ = false;
            cb_(buffer_);
        }
    });
}

void TextInput::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    text_.set_pos(pos - bg_->get_origin() + sf::Vector2f{padding_, 0});
}

void TextInput::draw(Window& window) const {
    window.draw(*bg_);
    window.draw(text_);

    auto mouse_pos = window.pixel_to_coords(sf::Mouse::getPosition(window.base()));
    if (get_bounds().contains(mouse_pos)) {
        window.set_mouse_cursor(sf::Cursor::Text);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            is_focused_ = true;
        }
    } else {
        if (is_focused_ && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            is_focused_ = false;
        }
    }
}

TextInput::~TextInput() {
    window_.remove_handler("text_input_handler");
}

#include "GameOver.hpp"

engine::GameOver::GameOver(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& retry,
                           Button&& quit)
    : bg_(std::move(bg)), msg_(std::move(msg)), retry_(std::move(retry)), quit_(std::move(quit)) {
    bg_->set_size(retry_.get_size() + quit_.get_size() + sf::Vector2f{margin_ + offset_ * 2, offset_ * 2 + margin_},
                  false);

    msg_.set_origin(msg_.get_size() / 2);

    retry_.set_cb([] {
        GameBus::get().emit(GameEvent::GameRestarted, nullptr);
    });

    quit_.set_cb([] {
        GameBus::get().emit(GameEvent::WindowClosed, nullptr);
    });

    set_pos(window.get_center() - get_size() / 2);
}

void engine::GameOver::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    msg_.set_pos(bg_->pos() + sf::Vector2f{bg_->get_size().x / 2, offset_});
    quit_.set_pos(bg_->pos() + sf::Vector2f{offset_, bg_->get_size().y - offset_});
    retry_.set_pos(quit_.pos() + sf::Vector2f{margin_, 0});
}

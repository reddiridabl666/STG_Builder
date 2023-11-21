#include "GameOver.hpp"

engine::GameOver::GameOver(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& retry,
                           Button&& quit, float offset, float margin)
    : bg_(std::move(bg)),
      msg_(std::move(msg)),
      retry_(std::move(retry)),
      quit_(std::move(quit)),
      offset_(offset),
      margin_(margin) {
    // auto bg_x = bg_->get_size().x;
    auto bg_x = 2 * std::max(retry_.get_size().x, quit_.get_size().x) + margin_ + 2 * offset_;
    auto bg_y = std::max(retry_.get_size().y, quit_.get_size().y) + margin_ + offset_ + msg_.get_size().y;

    bg_->set_size(sf::Vector2f{bg_x, bg_y}, false);

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
    quit_.set_pos(bg_->pos() +
                  sf::Vector2f{bg_->get_size().x / 2 - quit_.get_size().x - margin_ / 2, msg_.get_size().y + margin_});
    retry_.set_pos(quit_.pos() + sf::Vector2f{margin_ + quit_.get_size().x, 0});
}
#include "MainMenu.hpp"

#include "UiUtils.hpp"

MainMenu::MainMenu(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& start,
                   Button&& settings, Button&& quit, float offset)

    : bg_(std::move(bg)),
      msg_(std::move(msg)),
      start_(std::move(start)),
      settings_(std::move(settings)),
      quit_(std::move(quit)),
      offset_(offset) {
    normalise_width(start_, settings_, quit_);

    bg_->set_height(quit_.height() + start_.height() + msg_.height() + settings_.height() + offset * 5);

    msg_.set_origin(msg_.get_size() / 2);
    start_.set_origin(start_.get_size() / 2);
    settings_.set_origin(settings_.get_size() / 2);
    quit_.set_origin(quit_.get_size() / 2);

    // continue_.set_cb([] {
    //     GameBus::get().emit(GameEvent::GameUnpaused, nullptr);
    // });

    // quit_.set_cb([] {
    //     GameBus::get().emit(GameEvent::GameEnded, nullptr);
    // });

    set_pos(window.get_center() - bg_->get_size() / 2);
}

void MainMenu::draw(Window& window) {
    bg_->draw(window);
    msg_.draw(window);
    start_.draw(window);
    settings_.draw(window);
    quit_.draw(window);
}

void MainMenu::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    msg_.set_pos(bg_->pos() + sf::Vector2f{bg_->width() / 2, offset_});
    start_.set_pos(msg_.pos() + sf::Vector2f{0, offset_ + msg_.height()});
    settings_.set_pos(start_.pos() + sf::Vector2f{0, offset_ + start_.height()});
    quit_.set_pos(settings_.pos() + sf::Vector2f{0, offset_ + settings_.height()});
}

#include "PauseMenu.hpp"

#include "GameBus.hpp"

engine::PauseMenu::PauseMenu(const Window& window, std::unique_ptr<Displayable>&& bg, Text&& msg, Button&& continue_btn,
                             Button&& quit, float offset)
    : bg_(std::move(bg)),
      msg_(std::move(msg)),
      continue_(std::move(continue_btn)),
      quit_(std::move(quit)),
      offset_(offset) {
    bg_->set_height(quit_.height() + continue_.height() + msg_.height() + offset * 4);

    auto max_width = std::max(continue_.width(), quit_.width());
    quit_.set_width(max_width);
    continue_.set_width(max_width);

    msg_.set_origin(msg_.get_size() / 2);
    continue_.set_origin(continue_.get_size() / 2);
    quit_.set_origin(quit_.get_size() / 2);

    continue_.set_cb([] {
        GameBus::get().emit(GameEvent::GameUnpaused, nullptr);
    });

    quit_.set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    set_pos(window.get_center() - bg_->get_size() / 2);
}

void engine::PauseMenu::draw(Window& window) {
    bg_->draw(window);
    msg_.draw(window);
    continue_.draw(window);
    quit_.draw(window);
}

void engine::PauseMenu::set_pos(const sf::Vector2f& pos) {
    bg_->set_pos(pos);
    msg_.set_pos(bg_->pos() + sf::Vector2f{bg_->width() / 2, offset_});
    continue_.set_pos(msg_.pos() + sf::Vector2f{0, offset_ + msg_.height() / 2 + continue_.height() / 2});
    quit_.set_pos(continue_.pos() + sf::Vector2f{0, offset_ + continue_.height() / 2 + quit_.height() / 2});
};

#include "PauseMenu.hpp"

#include "GameBus.hpp"

engine::PauseMenu::PauseMenu(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                             std::unique_ptr<Button>&& continue_btn, std::unique_ptr<Button>&& quit, float offset)
    : layout_(offset) {
    continue_btn->set_cb([] {
        GameBus::get().emit(GameEvent::GameUnpaused, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    layout_.set_container(std::move(bg));

    std::vector<std::unique_ptr<Displayable>> items;
    items.reserve(3);

    items.push_back(std::move(msg));
    items.push_back(std::move(continue_btn));
    items.push_back(std::move(quit));

    layout_.set_items(std::move(items));
    layout_.init(1);

    set_pos(window.get_center() - layout_.get_size() / 2);
}

void engine::PauseMenu::draw(Window& window) {
    layout_.draw(window);
}

void engine::PauseMenu::set_pos(const sf::Vector2f& pos) {
    layout_.set_pos(pos);
};

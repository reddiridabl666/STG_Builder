#include "MainMenu.hpp"

#include "GameBus.hpp"

MainMenu::MainMenu(const Window& window, std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                   std::unique_ptr<Button>&& start, std::unique_ptr<Button>&& settings, std::unique_ptr<Button>&& quit,
                   float offset)
    : layout_(offset) {
    start->set_cb([] {
        GameBus::get().emit(GameEvent::GameStarted, nullptr);
    });

    settings->set_cb([] {
        //     GameBus::get().emit(GameEvent::SettingsOpened, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::GameEnded, nullptr);
    });

    layout_.set_container(std::move(bg));

    std::vector<std::unique_ptr<Displayable>> items;
    items.reserve(4);

    items.push_back(std::move(msg));
    items.push_back(std::move(start));
    items.push_back(std::move(settings));
    items.push_back(std::move(quit));

    layout_.set_items(std::move(items));
    layout_.init(1);

    set_pos(window.get_center() - layout_.get_size() / 2);
}

void MainMenu::draw(Window& window) {
    layout_.draw(window);
}

void MainMenu::set_pos(const sf::Vector2f& pos) {
    layout_.set_pos(pos);
}

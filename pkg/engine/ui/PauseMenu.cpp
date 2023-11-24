#include "PauseMenu.hpp"

#include "GameBus.hpp"

engine::PauseMenu::PauseMenu(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                             std::unique_ptr<Button>&& continue_btn, std::unique_ptr<Button>&& quit, float offset)
    : Form<VerticalLayout>(offset) {
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
}

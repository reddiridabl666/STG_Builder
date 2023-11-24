#include "PauseMenu.hpp"

#include "GameBus.hpp"

engine::PauseMenu::PauseMenu(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                             std::unique_ptr<Button>&& continue_btn, std::unique_ptr<Button>&& quit, float offset)
    : Form<VerticalLayout>(std::move(bg), offset) {
    continue_btn->set_cb([] {
        GameBus::get().emit(GameEvent::GameUnpaused, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    layout_.add_items(std::move(msg), std::move(continue_btn), std::move(quit));
    layout_.init(1);
}

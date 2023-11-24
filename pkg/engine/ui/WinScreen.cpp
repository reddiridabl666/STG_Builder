#include "WinScreen.hpp"

#include "DummyDisplayable.hpp"
#include "GameBus.hpp"
#include "HorizontalLayout.hpp"

engine::WinScreen::WinScreen(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                             std::unique_ptr<Button>&& save_score, std::unique_ptr<Button>&& quit, float offset_x,
                             float offset_y)
    : Form<VerticalLayout>(std::move(bg), offset_y) {
    save_score->set_cb([] {
        GameBus::get().emit(GameEvent::ScoreInputOpened, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    auto horizontal = std::make_unique<HorizontalLayout>(std::make_unique<DummyDisplayable>(), offset_x,
                                                         std::move(quit), std::move(save_score));

    layout_.add_items(std::move(msg), std::move(horizontal));
    layout_.init(2);
}

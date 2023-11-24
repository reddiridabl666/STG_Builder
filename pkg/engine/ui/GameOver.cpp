#include "GameOver.hpp"

#include "DummyDisplayable.hpp"
#include "GameBus.hpp"
#include "HorizontalLayout.hpp"

engine::GameOver::GameOver(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                           std::unique_ptr<Button>&& retry, std::unique_ptr<Button>&& quit, float offset, float margin)
    : Form<VerticalLayout>(std::move(bg), margin) {
    retry->set_cb([] {
        GameBus::get().emit(GameEvent::GameRestarted, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    auto horizontal = std::make_unique<HorizontalLayout>(std::make_unique<DummyDisplayable>(), offset, std::move(quit),
                                                         std::move(retry));

    layout_.add_items(std::move(msg), std::move(horizontal));
    layout_.init(2);
}

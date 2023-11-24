#include "GameOver.hpp"

#include "DummyDisplayable.hpp"
#include "GameBus.hpp"
#include "HorizontalLayout.hpp"

engine::GameOver::GameOver(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                           std::unique_ptr<Button>&& retry, std::unique_ptr<Button>&& quit, float offset, float margin)
    : Form<VerticalLayout>(margin) {
    retry->set_cb([] {
        GameBus::get().emit(GameEvent::GameRestarted, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::MainMenuOpened, nullptr);
    });

    std::vector<std::unique_ptr<Displayable>> btns;
    btns.reserve(2);

    btns.push_back(std::move(quit));
    btns.push_back(std::move(retry));

    auto horizontal = std::make_unique<HorizontalLayout>(std::make_unique<DummyDisplayable>(), std::move(btns), offset);

    std::vector<std::unique_ptr<Displayable>> items;
    items.reserve(2);

    items.push_back(std::move(msg));
    items.push_back(std::move(horizontal));

    layout_.set_container(std::move(bg));
    layout_.set_items(std::move(items));
    layout_.init(2);
}

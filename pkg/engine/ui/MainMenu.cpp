#include "MainMenu.hpp"

#include "GameBus.hpp"

namespace engine {
MainMenu::MainMenu(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<Button>&& start,
                   std::unique_ptr<Button>&& settings, std::unique_ptr<Button>&& leaderboards,
                   std::unique_ptr<Button>&& quit, float offset)
    : Form<VerticalLayout>(std::move(bg), offset) {
    start->set_cb([] {
        GameBus::get().emit(GameEvent::GameStarted, nullptr);
    });

    settings->set_cb([] {
        //     GameBus::get().emit(GameEvent::SettingsOpened, nullptr);
    });

    leaderboards->set_cb([] {
        GameBus::get().emit(GameEvent::LeaderboardsOpened, nullptr);
    });

    quit->set_cb([] {
        GameBus::get().emit(GameEvent::GameEnded, nullptr);
    });

    layout_.add_items(std::move(msg), std::move(start), std::move(settings), std::move(leaderboards), std::move(quit));
    layout_.init(1);
}
}  // namespace engine

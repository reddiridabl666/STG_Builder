#include "GameFactory.hpp"

namespace engine {
namespace {
GameOver create_game_over(const Window& window, const nl::json& json, assets::Manager& assets) {
    auto btn_style = json.value("button_style", nl::json::object());
    return GameOver{
        window,
        DisplayableFactory::create(json.at("bg"), assets.textures()),
        TextFactory::create_unique(json.at("message"), assets.fonts()),
        ButtonFactory::create_unique(json.value("retry", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("quit", ""), btn_style, assets),
        json.value("offset_x", 50.f),
        json.value("offset_y", 100.f),
    };
}

PauseMenu create_pause_menu(const Window& window, const nl::json& json, assets::Manager& assets) {
    auto btn_style = json.value("button_style", nl::json::object());
    return PauseMenu{
        window,
        DisplayableFactory::create(json.at("bg"), assets.textures()),
        TextFactory::create_unique(json.value("message", nl::json::object()), assets.fonts()),
        ButtonFactory::create_unique(json.value("continue", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("quit", ""), btn_style, assets),
        json.value("offset", 50.f),
    };
}

MainMenu create_main_menu(const Window& window, const nl::json& json, assets::Manager& assets) {
    auto btn_style = json.value("button_style", nl::json::object());
    return MainMenu{
        window,
        DisplayableFactory::create(json.at("bg"), assets.textures()),
        TextFactory::create_unique(json.value("message", nl::json::object()), assets.fonts()),
        ButtonFactory::create_unique(json.value("start", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("settings", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("quit", ""), btn_style, assets),
        json.value("offset", 50.f),
    };
}

WinScreen create_win_screen(const Window& window, const nl::json& json, assets::Manager& assets) {
    auto btn_style = json.value("button_style", nl::json::object());
    return WinScreen{
        window,
        DisplayableFactory::create(json.at("bg"), assets.textures()),
        TextFactory::create_unique(json.at("message"), assets.fonts()),
        ButtonFactory::create_unique(json.value("score", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("quit", ""), btn_style, assets),
        json.value("offset_x", 50.f),
        json.value("offset_y", 100.f),
    };
}
}  // namespace

SideMenu GameFactory::create_side_menu(const Window& window, const nl::json& menu, assets::Manager& assets) {
    auto props = menu.get<SideMenuProps>();
    return SideMenu{
        window,
        assets.textures().get_or(props.bg, assets::kFallbackImage),
        props,
    };
}

GameUi GameFactory::create_game_ui(const Window& window, const nl::json& game, assets::Manager& assets) {
    return GameUi{
        .game_over = create_game_over(window, game.value("game_over", nl::json::object()), assets),
        .pause_menu = create_pause_menu(window, game.value("pause_menu", nl::json::object()), assets),
        .main_menu = create_main_menu(window, game.value("main_menu", nl::json::object()), assets),
        .win_screen = create_win_screen(window, game.value("win_screen", nl::json::object()), assets),
    };
}

}  // namespace engine

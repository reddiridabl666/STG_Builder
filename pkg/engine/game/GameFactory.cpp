#include "GameFactory.hpp"

namespace engine {
GameOver GameFactory::create_game_over(const Window& window, const nl::json& json, assets::Manager& assets) {
    auto btn_style = json.value("button_style", nl::json::object());
    return GameOver{
        window,
        DisplayableFactory::create(json.at("bg"), assets.textures()),
        TextFactory::create_unique(json.at("message"), assets.fonts()),
        ButtonFactory::create_unique(json.value("retry", ""), btn_style, assets),
        ButtonFactory::create_unique(json.value("quit", ""), btn_style, assets),
        json.value("offset", 50.f),
        json.value("margin", 100.f),
    };
}

PauseMenu GameFactory::create_pause_menu(const Window& window, const nl::json& json, assets::Manager& assets) {
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

MainMenu GameFactory::create_main_menu(const Window& window, const nl::json& json, assets::Manager& assets) {
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

SideMenu GameFactory::create_side_menu(const Window& window, const nl::json& menu, assets::Manager& assets) {
    auto props = menu.get<SideMenuProps>();
    return SideMenu{
        window,
        assets.textures().get_or(props.bg, assets::kFallbackImage),
        props,
    };
}
}  // namespace engine

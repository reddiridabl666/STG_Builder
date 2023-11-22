#pragma once

#include "App.hpp"
#include "ButtonFactory.hpp"

namespace engine {
struct AppFactory {
    static App create(Window& window, const nl::json& main_menu, const std::filesystem::path& game_dir) {
        auto btn_style = main_menu.value("button_style", nl::json::object());

        auto main_menu = MainMenu{
            window,
            DisplayableFactory::create(main_menu.at("bg"), assets.textures()),
            TextFactory::create(main_menu.at("message"), assets.fonts()),
            ButtonFactory::create(main_menu.value("start", ""), btn_style, assets),
            ButtonFactory::create(main_menu.value("settings", ""), btn_style, assets),
            ButtonFactory::create(main_menu.value("quit", ""), btn_style, assets),
            main_menu.value("offset", 50.f),
        };

        auto app = engine::App{window, game_dir};
    };
}  // namespace engine

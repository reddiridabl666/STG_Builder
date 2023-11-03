#pragma once

#include "Game.hpp"
#include "SideMenu.hpp"

namespace engine {
class GameFactory {
  public:
    template <typename GameType>
    static GameType generate(Window& window, const nl::json& game, const nl::json& entities,
                             const std::string& base_dir);

    template <typename GameType>
    static std::unique_ptr<GameType> generate_unique(Window& window, const nl::json& game, const nl::json& entities,
                                                     const std::string& base_dir);

  private:
    static SideMenu create_side_menu(const Window& window, const nl::json& menu, assets::Manager& assets);
};

template <typename GameType = Game<>>
GameType GameFactory::generate(Window& window, const nl::json& game, const nl::json& entities,
                               const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);

    // TODO: do something about the side menu
    // TODO: should directories be hardcoded?

    const auto img_path = base_dir + "/assets/images";

    assets::Manager manager(img_path, base_dir + "/assets/sounds", base_dir + "/assets/fonts");

    LevelLoader level_loader(base_dir + "/level", game.value("field_size", GameField::kDefaultRatio), img_path);

    LevelManager levels(game.value("levels", 0), std::move(level_loader));

    return GameType{
        window,
        SpriteObject(manager.textures().get_or(game.value("bg", ""), assets::kFallbackImage)),
        create_side_menu(window, game.at("side_menu"), manager),
        PlayerLoader(game.at("players")),
        std::move(manager),
        std::move(types),
        std::move(levels),
        game.value("fps", 60),
    };
}

template <typename GameType>
std::unique_ptr<GameType> GameFactory::generate_unique(Window& window, const nl::json& game, const nl::json& entities,
                                                       const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);

    const auto img_path = base_dir + "/assets/images";
    assets::Manager manager(img_path, base_dir + "/assets/sounds", base_dir + "/assets/fonts");

    LevelLoader level_loader(base_dir + "/level", game.value("field_size", GameField::kDefaultRatio), img_path);

    LevelManager levels(game.value("levels", 0), std::move(level_loader));

    auto fps = game.value("fps", 60);

    // clang-format off
    return std::make_unique<GameType>(
        window,
        SpriteObject(manager.textures().get_or(game.value("bg", ""), assets::kFallbackImage)),
        create_side_menu(window, game.at("side_menu"), manager),
        PlayerLoader(game.at("players")),
        std::move(manager),
        std::move(types),
        std::move(levels),
        fps
    );
    // clang-format on
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

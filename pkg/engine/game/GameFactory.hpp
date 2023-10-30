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
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    // TODO: do something about the side menu
    // TODO: should directories be hardcoded?
    const auto img_path = base_dir + "/assets/images";

    assets::Manager manager(img_path, base_dir + "/assets/sounds", base_dir + "/assets/fonts");

    LevelLoader level_loader(base_dir + "/level",
                             json::get<sf::FloatRect>(game, "field_size", GameField::kDefaultRatio), img_path);

    LevelManager levels(json::get<int>(game, "levels"), std::move(level_loader));

    return GameType{
        window,
        SpriteObject(manager.textures().get_or(json::get<std::string>(game, "bg"), assets::kFallbackImage)),
        create_side_menu(window, game.at("side_menu"), manager),
        PlayerLoader(game.at("players")),
        std::move(manager),
        std::move(types.value()),
        std::move(levels),
        json::get<int>(game, "fps", 60),
    };
}

template <typename GameType>
std::unique_ptr<GameType> GameFactory::generate_unique(Window& window, const nl::json& game, const nl::json& entities,
                                                       const std::string& base_dir) {
    auto types = ObjectTypeFactory::generate(entities);
    if (!types) {
        throw std::runtime_error(types.error().message());
    }

    const auto img_path = base_dir + "/assets/images";
    assets::Manager manager(img_path, base_dir + "/assets/sounds", base_dir + "/assets/fonts");

    LevelLoader level_loader(base_dir + "/level",
                             json::get<sf::FloatRect>(game, "field_size", GameField::kDefaultRatio), img_path);
    LevelManager levels(json::get<int>(game, "levels"), std::move(level_loader));

    auto fps = json::get<int>(game, "fps", 60);

    // clang-format off
    return std::make_unique<GameType>(
        window,
        SpriteObject(manager.textures().get_or(json::get<std::string>(game, "bg"), assets::kFallbackImage)),
        create_side_menu(window, game.at("side_menu"), manager),
        PlayerLoader(game.at("players")),
        std::move(manager),
        std::move(types.value()),
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

#pragma once

#include "ButtonFactory.hpp"
#include "DisplayableFactory.hpp"
#include "Game.hpp"
#include "TextFactory.hpp"
#include "ui/GameOver.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/SideMenu.hpp"

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
    static GameOver create_game_over(const Window& window, const nl::json& json, assets::Manager& assets);
    static PauseMenu create_pause_menu(const Window& window, const nl::json& json, assets::Manager& assets);
    static MainMenu create_main_menu(const Window& window, const nl::json& json, assets::Manager& assets);

    template <typename T, typename Constructor>
    static T create(Window& window, const nl::json& game, const nl::json& entities, const std::string& base_dir,
                    Constructor&& constructor);
};

template <typename T>
struct BindConstructor {
    template <typename... Args>
    T operator()(Args&&... args) const {
        return T(std::forward<Args>(args)...);
    }
};

template <typename T>
struct BindMakeUnique {
    template <typename... Args>
    std::unique_ptr<T> operator()(Args&&... args) const {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};

template <typename GameType = Game<>>
inline GameType GameFactory::generate(Window& window, const nl::json& game, const nl::json& entities,
                                      const std::string& base_dir) {
    return create<GameType>(window, game, entities, base_dir, BindConstructor<GameType>{});
}

template <typename GameType = Game<>>
inline std::unique_ptr<GameType> GameFactory::generate_unique(Window& window, const nl::json& game,
                                                              const nl::json& entities, const std::string& base_dir) {
    return create<std::unique_ptr<GameType>>(window, game, entities, base_dir, BindMakeUnique<GameType>{});
}

template <typename T, typename Constructor>
inline T GameFactory::create(Window& window, const nl::json& game, const nl::json& entities,
                             const std::string& base_dir, Constructor&& constructor) {
    auto types = ObjectTypeFactory::generate(entities);

    const auto img_path = base_dir + "/assets/images";
    assets::Manager manager(img_path, base_dir + "/assets/sounds", base_dir + "/assets/fonts");

    LevelLoader level_loader(base_dir + "/level", game.value("field_size", GameField::kDefaultRatio), img_path);

    LevelManager levels(game.value("levels", 0), std::move(level_loader));

    auto fps = game.value("fps", 60);

    // clang-format off
    return constructor(
        window,
        SpriteObject(manager.textures().get_or(game.value("bg", ""), assets::kFallbackImage)),
        create_side_menu(window, game.value("side_menu", nl::json::object()), manager),
        create_main_menu(window, game.value("main_menu", nl::json::object()), manager),
        create_game_over(window, game.value("game_over", nl::json::object()), manager),
        create_pause_menu(window, game.value("pause_menu", nl::json::object()), manager),
        PlayerLoader(game.value("players", nl::json::array()), game.value("player_marker", PlayerMarkerProps{})),
        std::move(manager),
        std::move(types),
        std::move(levels),
        fps
    );
    // clang-format on
}
}  // namespace engine

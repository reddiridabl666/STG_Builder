#include "GameBuilder.hpp"

#include <fstream>

#include "GameFactory.hpp"
#include "ui/elements/GameInfo.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace builder {
void GameBuilder::init(const fs::path& game_dir) {
    auto game = json::read(game_dir / kGame);
    if (!game) {
        throw std::runtime_error(game.error().message());
    }

    auto entities = json::read(game_dir / kEntities);
    if (!entities) {
        throw std::runtime_error(entities.error().message());
    }

    std::vector<nl::json> levels;
    for (const auto& file : fs::directory_iterator(game_dir)) {
        fs::path filename = file.path().filename();

        if (!filename.string().starts_with("level") || !file.is_regular_file()) {
            continue;
        }

        auto level = json::read(game_dir / filename);
        if (!level) {
            throw std::runtime_error(level.error().message());
        }
        levels.push_back(std::move(*level));
    }

    *this = GameBuilder(game_dir, std::move(*game), std::move(*entities), std::move(levels));
}

fs::path GameBuilder::create_unique_filename(const fs::path& path) {
    if (fs::exists(path)) {
        return create_unique_filename(path.string() + " (1)");
    }
    return path;
}

fs::path GameBuilder::new_game(const fs::path& game) const {
    auto path = create_unique_filename(game);

    if (!fs::create_directory(path)) {
        throw std::runtime_error("Game directory creation failure");
    }

    if (!fs::create_directories(path / "assets/images")) {
        throw std::runtime_error("Assets directory creation failure");
    }

    if (!fs::create_directories(path / "assets/fonts")) {
        throw std::runtime_error("Assets directory creation failure");
    }

    fs::copy(path.parent_path() / assets::kFallbackImage, path / "assets/images");
    fs::copy(path.parent_path() / assets::kFallbackFont, path / "assets/fonts");

    nl::json game_json{
        {"name", path.stem().string()},
        {"description", ui::GameInfo::kDefaultDesc},
        {"levels", 0},
        {"field_size", GameField::kDefaultRatio},
        {"players", nl::json::array({
                        {"image", assets::kFallbackImage},
                        {"size", sf::Vector2f{100, 100}},
                        {"speed", 300},
                    })},
        {"side_menu", }
    };

    json::create(path / "game.json", game_json);

    json::create(path / "entities.json");

    return path;
}

void GameBuilder::save() const {
    if (game_dir_ == "") {
        return;
    }

#ifdef DEBUG
    LOG("Saving game " + game_dir_.string());
#endif

    json::create(game_dir_ / kGame, game_);
    json::create(game_dir_ / kEntities, entities_);

    size_t i = 1;
    for (const auto& level : levels_) {
        json::create(game_dir_ / level_filename(i), level);
        ++i;
    }
}

void GameBuilder::choose_level(size_t num) {
    current_level_ = num;
}

void GameBuilder::new_level() {
    current_level_ = levels_.size();
    nl::json level = {
        {"name", "New level"},
        {"entities", nl::json::array()},
        {"bg",
         {
             {"image", assets::kFallbackImage},
             {"speed", 100.0},
         }},
    };
    json::create(game_dir_ / level_filename(current_level_ + 1), level);
    levels_.push_back(std::move(level));
    game_["levels"] = levels_.size();
}

void GameBuilder::backup(const fs::path& path) {
    auto backup_path = game_dir_.parent_path() / ".backup";
    fs::create_directory(backup_path);
    fs::copy(path, backup_path / path.filename());
}

std::unique_ptr<EditableGame> GameBuilder::create_engine(Window& window) {
    return engine::GameFactory::generate_unique<EditableGame>(window, game_, entities_, game_dir_);
}

void GameBuilder::delete_game() {
    // backup(game_dir_);
    fs::remove_all(game_dir_);
    game_dir_ = "";
}

void GameBuilder::delete_level() {
    // backup(game_dir_);
    fs::remove(game_dir_ / level_filename(current_level_ + 1));
    levels_.erase(levels_.begin() + current_level_);  // TODO: Dangerous
    game_["levels"] = levels_.size();
}
}  // namespace builder

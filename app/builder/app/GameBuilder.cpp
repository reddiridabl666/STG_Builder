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

void GameBuilder::new_game(const fs::path& game) const {
    if (!fs::create_directory(game)) {
        throw std::runtime_error("Game directory creation failure");
    }

    if (!fs::create_directories(game / "assets/images")) {
        throw std::runtime_error("Assets directory creation failure");
    }

    nl::json game_json{
        {"name", game.stem().string()},
        {"description", ui::GameInfo::kDefaultDesc},
    };

    json::create(game / "game.json", game_json);

    json::create(game / "entities.json");
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
    current_level_ = levels_.size() + 1;
    auto level = R"({"name":"New level"})"_json;
    json::create(game_dir_ / level_filename(current_level_), level);
    levels_.push_back(std::move(level));
}

void GameBuilder::backup(const fs::path& path) {
    auto backup_path = game_dir_.parent_path() / ".backup";
    fs::create_directory(backup_path);
    fs::copy(path, backup_path / path.filename());
}

std::unique_ptr<engine::Game> GameBuilder::create_engine(Window& window) {
    return std::make_unique<engine::Game>(engine::GameFactory::generate(window, game_, entities_, game_dir_));
}

void GameBuilder::delete_game() {
    // backup(game_dir_);
    fs::remove_all(game_dir_);
    game_dir_ = "";
}

void GameBuilder::delete_level() {
    // backup(game_dir_);
    fs::remove(level_filename(current_level_));
    levels_.erase(levels_.begin() + current_level_);  // TODO: Dangerous
}
}  // namespace builder

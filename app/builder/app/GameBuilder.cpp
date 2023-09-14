#include "GameBuilder.hpp"

#include <fstream>

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace builder {
GameBuilder GameBuilder::init(const fs::path& game_dir) {
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

    return GameBuilder(game_dir, std::move(*game), std::move(*entities), std::move(levels));
}

void GameBuilder::save() const {
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
}  // namespace builder

#pragma once

#include <filesystem>

#include "Game.hpp"
#include "Json.hpp"
#include "ObjectOptions.hpp"
#include "ObjectType.hpp"

namespace builder {
namespace fs = std::filesystem;

class GameBuilder {
  public:
    GameBuilder() = default;

    GameBuilder(const fs::path& game_dir, nl::json&& game, nl::json&& entities,
                std::vector<nl::json>&& levels)
        : game_dir_(game_dir),
          game_(std::move(game)),
          entities_(std::move(entities)),
          levels_(std::move(levels)) {}

    static GameBuilder init(const fs::path& game_dir);

    void save() const;

    static const inline fs::path kGame = "game.json";
    static const inline fs::path kEntities = "entities.json";

    std::unique_ptr<engine::Game> create_engine(Window& window);

    nl::json& game() {
        return game_;
    }

    nl::json& entities() {
        return entities_;
    }

    const auto& levels() const {
        return levels_;
    }

    nl::json& current_level() {
        return levels_[current_level_];
    }

    void choose_level(size_t num);
    void new_level();

  private:
    static std::string level_filename(size_t num) {
        return fmt::format("level_{}.json", num);
    }

    fs::path game_dir_;
    nl::json game_;
    nl::json entities_;

    size_t current_level_ = 0;
    std::vector<nl::json> levels_;
};
}  // namespace builder

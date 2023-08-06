#pragma once

#include <cstddef>

#include "EnemyObject.hpp"

class Game {
  private:
    Game() = default;

    size_t enemy_count_ = 0;

    friend EnemyObject::EnemyObject(std::unique_ptr<Displayable>&&, int, const Properties::Data&);
    friend EnemyObject::~EnemyObject();

  public:
    Game(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) = delete;

    static Game& info() {
        static Game info_;
        return info_;
    }

    size_t enemy_count() {
        return enemy_count_;
    }
};

#pragma once

#include <cstddef>

#include "GameField.hpp"
#include "Observable.hpp"

class Game : public Observable<int, size_t> {
  public:
    enum Event {
        ObjectCreated,
        ObjectDestroyed,
        LevelChanged
    };

  private:
    Game();

    const GameField* field_;

    size_t enemy_count_ = 0;
    size_t player_count_ = 0;

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

    size_t player_count() {
        return player_count_;
    }

    const GameField* field() const {
        return field_;
    }
};

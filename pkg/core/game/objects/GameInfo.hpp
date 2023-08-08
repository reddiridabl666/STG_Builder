#pragma once

#include <cstddef>

#include "Observable.hpp"

class Game : public Observable<int, int> {
  public:
    enum Event {
        ObjectCreated,
        ObjectDestroyed
    };

  private:
    Game();

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
};

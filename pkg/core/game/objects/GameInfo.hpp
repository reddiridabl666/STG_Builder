#pragma once

#include <cstddef>

#include "GameField.hpp"
#include "Observable.hpp"

class GameState : public Observable<int, size_t> {
  public:
    enum Event {
        ObjectCreated,
        ObjectDestroyed,
        LevelChanged
    };

  private:
    GameState();

    // const GameField* field_;

    size_t enemy_count_ = 0;
    size_t player_count_ = 0;

  public:
    GameState(const GameState& other) = delete;
    GameState(GameState&& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    GameState& operator=(GameState&& other) = delete;

    static GameState& get() {
        static GameState info_;
        return info_;
    }

    size_t enemy_count() {
        return enemy_count_;
    }

    size_t player_count() {
        return player_count_;
    }

    // const GameField* field() const {
    //     return field_;
    // }
};

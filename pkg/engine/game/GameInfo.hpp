#pragma once

#include <cstddef>

#include "GameField.hpp"
#include "GameObject.hpp"
#include "GameObjectTag.hpp"
#include "Observable.hpp"

class GameState : public Observable<int, GameObjectTag> {
  public:
    enum Event {
        ObjectCreated,
        ObjectDestroyed,
        LevelChanged
    };

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
        return players_.size();
    }

    void add_player(std::weak_ptr<const GameObject> player) {
        players_.push_back(player);
    }

    void reset() {
        enemy_count_ = 0;
        players_.clear();
    }

    const auto& players() const {
        return players_;
    }

    void clear_players() {
        players_.clear();
    }

    void erase_player(size_t id) {
        players_.erase(players_.begin() + id);
    }

  private:
    GameState();

    size_t enemy_count_ = 0;

    std::vector<std::weak_ptr<const GameObject>> players_;
};

#pragma once

#include <cstddef>

#include "GameField.hpp"
#include "GameObject.hpp"
#include "GameObjectTag.hpp"
#include "Observable.hpp"

class GameState : public Observable<int, GameObject> {
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

    size_t enemy_count();

    void reset();

    const std::vector<std::weak_ptr<const GameObject>>& objects_by_tag(GameObjectTag);

  private:
    GameState();

    std::unordered_map<GameObjectTag, std::vector<std::weak_ptr<const GameObject>>> objects_;
};

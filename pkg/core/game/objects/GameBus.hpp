#pragma once

#include "GameEvents.hpp"
#include "Json.hpp"
#include "Observable.hpp"

class GameBus : public Observable<GameEvent, nl::json> {
  public:
    GameBus(const GameBus& other) = delete;
    GameBus(GameBus&& other) = delete;
    GameBus& operator=(const GameBus& other) = delete;
    GameBus& operator=(GameBus&& other) = delete;

    static GameBus& get() {
        static GameBus info_;
        return info_;
    }

  private:
    GameBus() = default;
};

#include "GameState.hpp"

#include "GameObject.hpp"

GameState::GameState() {
    on(Event::ObjectCreated, [this](const auto& obj) {
        switch (obj.tag()) {
            case GameObjectTag::Enemy:
                enemy_count_ += 1;
                break;
            default:
                break;
        }
    });

    on(Event::ObjectDestroyed, [this](const auto& obj) {
        switch (obj.tag()) {
            case GameObjectTag::Enemy:
                enemy_count_ -= 1;
                break;
            default:
                break;
        }
    });
}

#include "GameInfo.hpp"

#include "GameObject.hpp"

GameState::GameState() {
    on(Event::ObjectCreated, [this](auto& obj_type) {
        switch (obj_type) {
            case GameObject::Tag::Enemy:
                enemy_count_ += 1;
                break;
            case GameObject::Tag::Player:
                player_count_ += 1;
        }
    });

    on(Event::ObjectDestroyed, [this](auto& obj_type) {
        switch (obj_type) {
            case GameObject::Tag::Enemy:
                enemy_count_ -= 1;
                break;
            case GameObject::Tag::Player:
                player_count_ -= 1;
        }
    });
}

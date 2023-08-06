#include "EnemyObject.hpp"

#include "GameInfo.hpp"

EnemyObject::EnemyObject(std::unique_ptr<Displayable>&& image, int speed, const Properties::Data& props)
    : GameObject(std::move(image), speed, GameObject::Tag::Enemy, props) {
    Game::info().enemy_count_ += 1;
}

EnemyObject::~EnemyObject() {
    Game::info().enemy_count_ -= 1;
}

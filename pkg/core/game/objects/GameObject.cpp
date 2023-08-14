#include "GameObject.hpp"

#include "GameInfo.hpp"

GameObject::GameObject(const std::string& name, const sf::Vector2f& size,
                       std::unique_ptr<Displayable>&& image, int speed, Tag tag, const Properties& props)
    : GameObjectBase(std::move(image), speed), name_(name), tag_(tag), props_(props) {
    transformable().setOrigin(get_size() / 2);
    set_size(size);
}

void GameObject::update(const GameField& field, float delta_time) {
    if (!is_active()) {
        return;
    }

    alive_ = life_update_(*this, field);
    move_(delta_time);
}

void GameObject::move_(float delta_time) {
    if (!move_update_) {
        return;
    }

    if (move_update_.type() == movement::Func::Type::Pos) {
        return set_pos(move_update_(*this, delta_time));
    }

    velocity_ = move_update_(*this, delta_time);
    move(speed_ * velocity_ * delta_time);
}

float GameObject::left() const {
    return pos().x - width() / 2;
}

float GameObject::right() const {
    return pos().x + width() / 2;
}

float GameObject::top() const {
    return pos().y - height() / 2;
}

float GameObject::bottom() const {
    return pos().y + height() / 2;
}

float GameObject::height() const {
    return get_size().y;
}

float GameObject::width() const {
    return get_size().x;
}

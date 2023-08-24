#include "GameObject.hpp"

#include "GameInfo.hpp"

GameObject::GameObject(const std::string& name, const sf::Vector2f& size,
                       std::unique_ptr<Displayable>&& image, int speed, Tag tag, const Properties& props)
    : ImageContainer(std::move(image), speed), name_(name), tag_(tag), props_(props) {
    transformable().setOrigin(get_size() / 2);
    set_size(size);
}

void GameObject::update(const GameField& field, float delta_time) {
    if (!update_activity(field)) {
        return;
    }

    alive_ = life_update_(*this, field);
    update_position(delta_time);
}

void GameObject::update_position(float delta_time) {
    if (!move_update_) {
        return;
    }

    if (move_update_.type() == movement::Func::Type::Pos) {
        return set_pos(move_update_(*this, delta_time));
    }

    velocity_ = move_update_(*this, delta_time);
    move(speed_ * velocity_ * delta_time);
}

bool GameObject::update_activity(const GameField& field) {
    if (is_active()) {
        return true;
    }

    if (name() == "enemy-0") {
        std::cout << "Start line:" << activity_start() << "\n";
        std::cout << "Cur center pos:" << field.center().y << "\n";
    }

    if (is_default_activatable()) {
        set_activity(field.is_in_bounds(*this));
    } else {
        set_activity(field.center().y <= activity_start());
    }

    return is_active();
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

const life::update GameObject::kDefaultLifeFunc = life::in_bounds(GameObject::kLoadDelta);

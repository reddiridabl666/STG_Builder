#include "GameObject.hpp"

#include "GameInfo.hpp"

GameObject::GameObject(const std::string& name, const sf::Vector2f& size,
                       std::unique_ptr<Displayable>&& image, int speed, Tag tag, const Properties& props,
                       float activity_start, const alive::update& life_func,
                       std::unique_ptr<movement::Rule>&& move_func, sf::Vector2f velocity, bool alive,
                       bool active)
    : ImageContainer(std::move(image), speed),
      name_(name),
      tag_(tag),
      props_(props),
      velocity_(velocity),
      move_update_(std::move(move_func)),
      life_update_(life_func),
      active_(active),
      alive_(alive),
      activity_start_(activity_start) {
    transformable().setOrigin(get_size() / 2);
    set_size(size);
}

GameObject::GameObject(GameObject&& other)
    : GameObject(std::move(other.name_), other.get_size(), std::move(other.image_), other.speed_, other.tag_,
                 std::move(other.props_), other.activity_start_, std::move(other.life_update_),
                 std::move(other.move_update_), other.velocity_, other.alive_, other.active_) {}

void GameObject::swap(GameObject& other) {
    std::swap(name_, other.name_);
    std::swap(speed_, other.speed_);
    std::swap(tag_, other.tag_);
    std::swap(props_, other.props_);
    std::swap(image_, other.image_);

    std::swap(move_update_, other.move_update_);
    std::swap(velocity_, other.velocity_);
    std::swap(alive_, other.alive_);
    std::swap(active_, other.active_);
    std::swap(life_update_, other.life_update_);
    std::swap(activity_start_, other.activity_start_);

    sf::Vector2f tmp = get_size();
    set_size(other.get_size());
    other.set_size(tmp);
}

GameObject& GameObject::operator=(GameObject&& other) {
    auto tmp = std::move(other);
    swap(tmp);
    return *this;
}

void GameObject::update(const GameField& field, float delta_time) {
    if (!update_activity(field)) {
        return;
    }

    alive_ = life_update_(*this, field);
    update_position(field, delta_time);
}

void GameObject::update_position(const GameField& field, float delta_time) {
    if (!move_update_ || !(*move_update_)) {
        return;
    }

    if (move_update_->moves_with_field()) {
        move(field.speed() * sf::Vector2f{0, -1} * delta_time);
    }

    if (move_update_->type() == movement::Rule::Type::Pos) {
        return set_pos(move_update_->operator()(*this, delta_time));
    }

    velocity_ = move_update_->operator()(*this, delta_time);
    move(speed_ * velocity_ * delta_time);
}

bool GameObject::update_activity(const GameField& field) {
    if (is_active()) {
        return true;
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

const alive::update GameObject::kDefaultLifeFunc = alive::in_bounds(GameObject::kLoadDelta);

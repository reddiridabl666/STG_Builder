#include "GameObject.hpp"

#include "CollisionReaction.hpp"
#include "GameField.hpp"

GameObject::GameObject(const std::string& name, const sf::Vector2f& size, std::unique_ptr<Displayable>&& image,
                       int speed, GameObjectTag tag, const Properties& props, float activity_start,
                       const alive::update& life_func, std::unique_ptr<movement::Rule>&& move_func,
                       std::unique_ptr<Hitbox>&& hitbox, CollisionReaction&& collision,
                       std::unique_ptr<action::Action>&& on_character_death, bool stop_at_bounds, sf::Vector2f velocity,
                       bool alive, bool active)
    : ImageContainer(std::move(image), speed),
      name_(name),
      tag_(tag),
      props_(props),
      velocity_(velocity),
      move_update_(std::move(move_func)),
      life_update_(life_func),
      //   hitbox_(std::move(hitbox)),
      collision_action_(std::move(collision)),
      on_character_death_(std::move(on_character_death)),
      active_(active),
      alive_(alive),
      stop_at_bounds_(stop_at_bounds),
      activity_start_(activity_start) {
    set_origin(get_size() / 2);
    set_size(size);
    hitbox_ = std::move(hitbox);
}

std::string GameObject::tag_to_str(GameObjectTag tag) {
    static const std::unordered_map<GameObjectTag, std::string> map = {OBJECT_TAG_TO_STRING};
    return map.at(tag);
}

void GameObject::update(const GameField& field, float delta_time) {
    if (!update_activity(field)) {
        return;
    }

    alive_ = life_update_(*this, field);
    update_position(field, delta_time);
}

std::string_view GameObject::type_name() const {
    return std::string_view(name_.begin(), name_.begin() + name_.rfind('-'));
}

void GameObject::resolve_collision(GameObject& other) {
    collision_action_(other, *this);
}

void GameObject::draw(Window& window) const {
    ImageContainer::draw(window);
    if (hitbox_ && !hitbox_->is_hidden()) {
        hitbox_->draw(window);
    }
}

void GameObject::set_rotation(float rotation) {
    ImageContainer::set_rotation(rotation);
    if (hitbox_) {
        hitbox_->set_rotation(rotation);
    }
}

void GameObject::scale(float x, float y) {
    ImageContainer::scale(x, y);
    if (hitbox_) {
        hitbox_->scale(x, y);
    }
}

void GameObject::set_pos(const sf::Vector2f& new_pos) {
    ImageContainer::set_pos(new_pos);
    if (hitbox_) {
        hitbox_->set_pos(new_pos);
    }
}

void GameObject::update_position(const GameField& field, float delta_time) {
    if (!move_update_ || !(*move_update_)) {
        return;
    }

    if (move_update_->moves_with_field()) {
        move(field.speed() * sf::Vector2f{0, -1} * delta_time);
    }

    auto result = move_update_->operator()(*this, delta_time);

    if (result.type == movement::Rule::Type::Pos) {
        set_pos(result.pos);
    } else {
        set_velocity(result.velocity);
        move(speed_ * velocity_ * delta_time);
    }

    if (stop_at_bounds_) {
        field.keep_in_bounds(*this);
    }
}

bool GameObject::update_activity(const GameField& field) {
    if (active_) {
        return true;
    }

    if (is_default_activatable()) {
        set_activity(field.is_in_bounds(*this));
    } else {
        set_activity(field.center().y <= activity_start());
    }

    if (active_ && move_update_) {
        move_update_->init(*this);
    }
    return active_;
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

void GameObject::emit(GameEvent event, const GameObject& other) {
    if (on_character_death_ && event == GameEvent::ObjectDestroyed) {
        on_character_death_->operator()(other, *this);
    }
}

const alive::update GameObject::kDefaultLifeFunc = alive::in_bounds(GameObject::kLoadDelta);

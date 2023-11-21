#include "Transformable.hpp"

const sf::Transformable& Transformable::transformable() const {
    return const_cast<Transformable*>(this)->transformable();
}

const sf::Vector2f& Transformable::get_origin() const {
    return transformable().getOrigin();
}

const sf::Vector2f& Transformable::pos() const {
    return transformable().getPosition();
}

void Transformable::set_pos(const sf::Vector2f& pos) {
    transformable().setPosition(pos);
}

void Transformable::set_pos(float x, float y) {
    set_pos(sf::Vector2f{x, y});
}

void Transformable::set_origin(const sf::Vector2f& origin) {
    transformable().setOrigin(origin);
}

void Transformable::set_width(float size, bool save_proportions) {
    float new_scale = size / get_size().x;

    if (save_proportions) {
        return scale(new_scale, new_scale);
    }

    scale(new_scale, 1);
}

void Transformable::set_size(const sf::Vector2f& size, bool save_proportions) {
    sf::Vector2f new_scale = size / get_size();

    if (save_proportions) {
        new_scale.y = new_scale.x;
    }

    scale(new_scale.x, new_scale.y);
}

void Transformable::scale(float x, float y) {
    transformable().scale(x, y);
}

void Transformable::move(const sf::Vector2f& offset) {
    set_pos(pos() + offset);
}

void Transformable::move(float x, float y) {
    move(sf::Vector2f{x, y});
}

void Transformable::set_rotation(float angle) {
    transformable().setRotation(angle);
}

void Transformable::rotate(float angle) {
    set_rotation(transformable().getRotation() + angle);
}

sf::Vector2f Transformable::original_size() const {
    return get_size() / transformable().getScale();
}

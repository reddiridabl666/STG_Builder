#include "Transformable.hpp"

float Transformable::height() const {
    return get_size().y;
}

float Transformable::width() const {
    return get_size().x;
}

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

void Transformable::set_width(float width, bool save_proportions) {
    set_size(sf::Vector2f{width, get_size().y}, save_proportions);
}

void Transformable::set_height(float height, bool save_proportions) {
    set_size(sf::Vector2f{width(), height}, save_proportions);
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

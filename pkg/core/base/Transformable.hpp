#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "LinAlg.hpp"

class Transformable {
  public:
    virtual sf::Transformable& transformable() = 0;

    const sf::Transformable& transformable() const {
        return const_cast<Transformable*>(this)->transformable();
    }

    const sf::Vector2f& pos() const {
        return transformable().getPosition();
    }

    void set_pos(const sf::Vector2f& pos) {
        transformable().setPosition(pos);
    }

    void set_size(const sf::Vector2f& size) {
        transformable().scale(size / get_size());
    }

    virtual sf::Vector2f get_size() const = 0;

    void move(const sf::Vector2f& offset) {
        transformable().move(offset);
    }

    void move(float x, float y) {
        move(sf::Vector2f{x, y});
    }

    virtual ~Transformable() = default;
};

#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

class Movable {
  public:
    virtual sf::Transformable& transformable() = 0;

    const sf::Transformable& transformable() const {
        return const_cast<Movable*>(this)->transformable();
    }

    const sf::Vector2f& pos() const {
        return transformable().getPosition();
    }

    void set_pos(const sf::Vector2f& pos) {
        transformable().setPosition(pos);
    }

    void move(const sf::Vector2f& pos) {
        transformable().move(pos);
    }

    virtual ~Movable() = default;
};

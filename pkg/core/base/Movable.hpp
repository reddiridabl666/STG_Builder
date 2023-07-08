#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

class Movable {
  public:
    virtual sf::Transformable& get_transformable() = 0;

    const sf::Transformable& get_transformable() const {
        return const_cast<Movable*>(this)->get_transformable();
    }

    const sf::Vector2f& pos() const {
        return get_transformable().getPosition();
    }

    void set_pos(const sf::Vector2f& pos) {
        get_transformable().setPosition(pos);
    }

    void move(const sf::Vector2f& pos) {
        get_transformable().move(pos);
    }

    virtual ~Movable() = default;
};

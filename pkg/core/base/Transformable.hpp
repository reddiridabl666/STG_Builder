#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>

#include "LinAlg.hpp"

class Transformable {
  public:
    virtual sf::Vector2f get_size() const = 0;

    virtual sf::FloatRect get_bounds() const = 0;

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

    void set_pos(float x, float y) {
        set_pos(sf::Vector2f{x, y});
    }

    void set_width(float size, bool save_proportions = true) {
        float scale = size / get_size().x;

        if (save_proportions) {
            return transformable().scale(scale, scale);
        }

        transformable().scale(scale, 1);
    }

    void set_size(const sf::Vector2f& size, bool save_proportions = true) {
        sf::Vector2f scale = size / get_size();

        if (save_proportions) {
            scale.y = scale.x;
        }

        transformable().scale(scale);
    }

    void move(const sf::Vector2f& offset) {
        transformable().move(offset);
    }

    void move(float x, float y) {
        move(sf::Vector2f{x, y});
    }

    void set_rotation(float angle) {
        transformable().setRotation(angle);
    }

    void rotate(float angle) {
        transformable().rotate(angle);
    }

    virtual ~Transformable() = default;
};

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

    const sf::Transformable& transformable() const;

    const sf::Vector2f& get_origin() const;

    const sf::Vector2f& pos() const;

    virtual void set_pos(const sf::Vector2f& pos);

    void set_pos(float x, float y);

    void set_origin(const sf::Vector2f& origin);

    void set_width(float size, bool save_proportions = true);

    virtual void set_size(const sf::Vector2f& size, bool save_proportions = true);

    virtual void scale(float x, float y);

    void move(const sf::Vector2f& offset);

    void move(float x, float y);

    virtual void set_rotation(float angle);

    void rotate(float angle);

    sf::Vector2f original_size() const;

    virtual ~Transformable() = default;
};

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

    virtual void set_pos(const sf::Vector2f& pos);

    virtual void set_size(const sf::Vector2f& size, bool save_proportions = true);

    virtual void scale(float x, float y);

    virtual void set_rotation(float angle);

    float height() const;

    float width() const;

    const sf::Transformable& transformable() const;

    const sf::Vector2f& get_origin() const;

    const sf::Vector2f& pos() const;

    void set_pos(float x, float y);

    void set_origin(const sf::Vector2f& origin);

    void set_width(float size, bool save_proportions = true);

    void set_height(float height, bool save_proportions = true);

    void move(const sf::Vector2f& offset);

    void move(float x, float y);

    void rotate(float angle);

    sf::Vector2f original_size() const;

    virtual ~Transformable() = default;
};

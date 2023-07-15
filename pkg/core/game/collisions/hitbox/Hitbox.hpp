#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Framed.hpp"
#include "Geometry.hpp"

class RectHitbox;
class CircleHitbox;

class Hitbox : public Framed {
  public:
    enum class Type {
        Circle,
        Rect
    };

    Hitbox(Type type) : type_(type) {}

    bool intersects_with(const Hitbox& other) const;

    Type get_type() const {
        return type_;
    }

  protected:
    virtual bool intersects_with_rect(const RectHitbox& other) const = 0;
    virtual bool intersects_with_circle(const CircleHitbox& other) const = 0;

  private:
    const Type type_;
};

class RectHitbox : public Hitbox {
  public:
    RectHitbox() : Hitbox(Hitbox::Type::Rect) {}
    virtual const FloatRect& rect() const = 0;

  protected:
    bool intersects_with_rect(const RectHitbox& other) const override;
    bool intersects_with_circle(const CircleHitbox& other) const override;

    friend CircleHitbox;
};

class CircleHitbox : public Hitbox {
  public:
    CircleHitbox() : Hitbox(Hitbox::Type::Circle) {}

    virtual float radius() const = 0;
    virtual const sf::Vector2f& center() const = 0;

  protected:
    bool intersects_with_rect(const RectHitbox& other) const override;
    bool intersects_with_circle(const CircleHitbox& other) const override;
};

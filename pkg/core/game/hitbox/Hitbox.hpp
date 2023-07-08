#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class RectHitbox;
class CircleHitbox;

class Hitbox {
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

  protected:
    virtual bool intersects_with_rect(const RectHitbox& other) const = 0;
    virtual bool intersects_with_circle(const CircleHitbox& other) const = 0;

  private:
    sf::FloatRect rect_;
};

class CircleHitbox : public Hitbox {
  public:
    CircleHitbox() : Hitbox(Hitbox::Type::Circle) {}

  protected:
    virtual bool intersects_with_rect(const RectHitbox& other) const = 0;
    virtual bool intersects_with_circle(const CircleHitbox& other) const = 0;

  private:
    sf::Vector2f radius_;
    sf::Vector2f center_;
};

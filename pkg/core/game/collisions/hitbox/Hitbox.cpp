#include "Hitbox.hpp"

#include "Geometry.hpp"

bool RectHitbox::intersects_with_rect(const RectHitbox& other) const {
    return geom::intersects(rect(), other.rect());
}

bool RectHitbox::intersects_with_circle(const CircleHitbox& other) const {
    return geom::distance(other.center(), rect()) < other.radius();
}

bool CircleHitbox::intersects_with_rect(const RectHitbox& other) const {
    return other.intersects_with_circle(*this);
}

bool CircleHitbox::intersects_with_circle(const CircleHitbox& other) const {
    return geom::distance(center(), other.center()) < radius() + other.radius();
}

bool Hitbox::intersects_with(const Hitbox& other) const {
    if (other.get_type() == Hitbox::Type::Rect) {
        return intersects_with_rect(dynamic_cast<const RectHitbox&>(other));
    }
    return intersects_with_circle(dynamic_cast<const CircleHitbox&>(other));
}

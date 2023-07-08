#include "Hitbox.hpp"

bool RectHitbox::intersects_with_rect(const RectHitbox& other) const {
    return rect_.intersects(other.rect_);
}

bool RectHitbox::intersects_with_circle(const CircleHitbox& other) const {}

bool CircleHitbox::intersects_with_rect(const RectHitbox& other) const {}

bool CircleHitbox::intersects_with_circle(const CircleHitbox& other) const {}

bool Hitbox::intersects_with(const Hitbox& other) const {
    if (other.get_type() == Hitbox::Type::Rect) {
        return intersects_with_rect(other);
    }
    return intersects_with_circle(other);
}

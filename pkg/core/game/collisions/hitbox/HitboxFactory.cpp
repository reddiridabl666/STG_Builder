#include "HitboxFactory.hpp"

#include "ShapeHitbox.hpp"

std::unique_ptr<Hitbox> HitboxFactory::create(const HitboxProps& props) {
    std::unique_ptr<Hitbox> hitbox;

    switch (props.type) {
        case Hitbox::Type::Rect:
            hitbox
                = std::make_unique<RectShapeHitbox>(props.size, props.outline_color, props.fill_color, props.outline);
            break;
        case Hitbox::Type::Circle:
            hitbox = std::make_unique<CircleShapeHitbox>(props.radius, props.outline_color, props.fill_color,
                                                         props.outline);
            break;
        default:
            return nullptr;
    }

    hitbox->set_visibility(!props.shown);
    return hitbox;
}

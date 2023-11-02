#include "HitboxFactory.hpp"

#include "ShapeHitbox.hpp"

std::unique_ptr<Hitbox> HitboxFactory::create(const HitboxProps& props) {
    auto hitbox = [&] -> std::unique_ptr<Hitbox> {
        switch (props.type) {
            case Hitbox::Type::Rect:
                return std::make_unique<RectShapeHitbox>(props.size, props.color, props.outline);
            case Hitbox::Type::Circle:
                return std::make_unique<CircleShapeHitbox>(props.radius, props.color, props.outline);
            default:
                return nullptr;
        }
    }();

    hitbox->set_visibility(props.hidden);
    return hitbox;
}

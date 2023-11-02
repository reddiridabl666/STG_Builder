#include <memory>

#include "HitboxProps.hpp"

struct HitboxFactory {
    static std::unique_ptr<Hitbox> create(const HitboxProps& props);
};

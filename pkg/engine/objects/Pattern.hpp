#pragma once

#include "AssetManager.hpp"

class GameObject;
class ObjectType;

class Pattern {
  public:
    using objects = std::vector<std::shared_ptr<GameObject>>;

    struct ObjectCount {
        virtual size_t get() const = 0;
        virtual ~ObjectCount() = default;
    };

    struct MovementSetter {
        virtual void set(const GameObject& parent, objects&) const = 0;
        virtual ~MovementSetter() = default;
    };

    struct PositionSetter {
        virtual void set(const GameObject& parent, objects& children) const = 0;
        virtual ~PositionSetter() = default;
    };

    using object_types = std::unordered_map<std::string, ObjectType>;

    virtual objects create(const std::shared_ptr<GameObject>& parent, object_types& types, assets::Manager& assets) = 0;
    virtual ~Pattern() = default;
};

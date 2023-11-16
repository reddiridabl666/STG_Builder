#pragma once

#include <limits>

#include "GameObject.hpp"
#include "LinAlg.hpp"

inline std::weak_ptr<const GameObject> find_nearest(const std::shared_ptr<GameObject>& to,
                                                    const std::vector<std::weak_ptr<const GameObject>>& objects) {
    float distance = std::numeric_limits<float>::max();
    std::weak_ptr<const GameObject> target;

    for (auto& obj : objects) {
        float new_distance = linalg::distance(obj.lock()->pos(), to->pos());
        if (distance > new_distance && obj.lock()->name() != to->name()) {
            distance = new_distance;
            target = obj;
        }
    }

    return target;
}

#include "ObjectOptions.hpp"

#include <algorithm>

void ObjectOptions::set_props(GameObject& obj) const {
    // TODO: ugly, should pass ObjectOptions to create_object and use props in constructor

    obj.set_pos(pos_x, pos_y);
    obj.props().append(props);
    obj.set_movement(move);

    if (life_func) {
        obj.set_life_update(life_func);
    }

    obj.set_rotation(rotation);
    obj.set_activity_start(activity_start);
}

std::ostream& operator<<(std::ostream& out, const ObjectOptions& opts) {
    return out << opts.type;
}

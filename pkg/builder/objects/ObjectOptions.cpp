#include "ObjectOptions.hpp"

#include <algorithm>

void ObjectOptions::set_props(GameObject& obj) const {
    obj.set_pos(pos_x, pos_y);
    obj.props().append(props);

    if (life_func) {
        obj.set_life_update(life_func);
    }

    obj.set_rotation(rotation);
}

std::ostream& operator<<(std::ostream& out, const ObjectOptions& opts) {
    return out << opts.type;
}

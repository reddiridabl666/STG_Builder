#include "ObjectOptions.hpp"

#include <algorithm>

void ObjectOptions::set_props(GameObject& obj) const {
    obj.set_pos(pos_x, pos_y);
    obj.set_movement(move);
    obj.props().append(props);
}

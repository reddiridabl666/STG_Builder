#include "ObjectOptions.hpp"

#include <algorithm>

void ObjectOptions::set_props(const GameField& field, GameObject& obj) {
    obj.set_pos(pos_x(field), pos_y(field));
    obj.set_movement(std::move(move));
    obj.props().append(props);
}

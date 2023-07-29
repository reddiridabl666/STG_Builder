#include "ObjectOptions.hpp"

#include <algorithm>

void ObjectOptions::set_props(const GameField& field, GameObject& obj) {
    obj.set_pos(pos_(field));
    obj.set_movement(std::move(move_));
    obj.props().append(props_);
}

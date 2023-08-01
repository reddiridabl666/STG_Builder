#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Properties.hpp"

struct ObjectOptions {
  public:
    ObjectOptions() = default;

    void set_props(const GameField& field, GameObject& obj);

    position::Func pos_x;
    position::Func pos_y;

    movement::Func move;
    Properties props;
};

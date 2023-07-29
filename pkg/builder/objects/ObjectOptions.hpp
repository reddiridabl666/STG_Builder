#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Properties.hpp"

class ObjectOptions {
  public:
    ObjectOptions(const position::Func& pos, const Properties& props, const movement::Func& move)
        : pos_(pos), move_(move), props_(props) {}

    void set_props(const GameField& field, GameObject& obj);

  private:
    position::Func pos_;
    movement::Func move_;
    Properties props_;
};

#pragma once

#include <fmt/ostream.h>

#include <SFML/System/Vector2.hpp>
#include <ostream>

#include "GameObject.hpp"
#include "Life.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Properties.hpp"

struct ObjectOptions {
  public:
    ObjectOptions() = default;

    void set_props(GameObject& obj) const;

    auto operator<=>(const ObjectOptions& other) const {
        return other.pos_y <=> pos_y;
    }

    std::string type;

    movement::Func move;
    alive::update life_func = GameObject::kDefaultLifeFunc;

    Properties props;

    float pos_x;
    float pos_y;

    float rotation = 0;

    float activity_start = GameObject::kDefaultActivityStart;
};

std::ostream& operator<<(std::ostream& out, const ObjectOptions& opts);

template <>
struct fmt::formatter<ObjectOptions> : fmt::ostream_formatter {};

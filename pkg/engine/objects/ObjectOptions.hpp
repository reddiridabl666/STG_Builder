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
    ObjectOptions(const std::string& type, const sf::Vector2f& pos) : type(type), pos_x(pos.x), pos_y(pos.y) {}

    ObjectOptions(ObjectOptions&&) = default;
    ObjectOptions& operator=(ObjectOptions&& other) = default;

    void set_props(GameObject& obj) const;

    auto operator<=>(const ObjectOptions& other) const {
        return other.pos_y <=> pos_y;
    }

    std::string type;

    std::unique_ptr<movement::Rule> move = movement::no_op();
    alive::update life_func = nullptr;

    Properties props;

    float pos_x;
    float pos_y;

    float rotation = 0;

    bool stop_at_bounds = false;
    size_t json_id = 0;

    float activity_start = GameObject::kDefaultActivityStart;
};

std::ostream& operator<<(std::ostream& out, const ObjectOptions& opts);

template <>
struct fmt::formatter<ObjectOptions> : fmt::ostream_formatter {};

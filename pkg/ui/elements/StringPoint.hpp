#pragma once

#include <string>

#include "GameField.hpp"
#include "Json.hpp"
#include "Messages.hpp"
#include "UiElement.hpp"

namespace ui {
struct StringPoint {
    StringPoint() = default;
    StringPoint(const sf::Vector2f& vec) : x_(vec.x), y_(vec.y) {}

    StringPoint& operator=(const sf::Vector2f& vec) {
        x_ = vec.x;
        y_ = vec.y;

        x_percent_ = false;
        y_percent_ = false;
        return *this;
    }

    void draw(const char* msg);

    sf::Vector2f to_vec(const GameField& field) const;

  private:
    float x_;
    float y_;

    int x_percent_ = 0;
    int y_percent_ = 0;

    friend void from_json(const nl::json&, StringPoint&);
    friend void to_json(nl::json& json, const StringPoint& res);
};
}  // namespace ui

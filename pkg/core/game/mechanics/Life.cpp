#include "Life.hpp"

#include <SFML/System/Clock.hpp>

#include "GameField.hpp"
#include "GameObject.hpp"

namespace alive {
update timed(float seconds) {
    return [seconds](const GameObject&, const GameField&) {
        static sf::Clock timer;
        if (timer.getElapsedTime().asSeconds() > seconds) {
            return false;
        }
        return true;
    };
}

update in_bounds(float margin) {
    return [margin](const GameObject& obj, const GameField& field) {
        return field.is_in_bounds(obj, margin);
    };
}
update default_func() {
    return in_bounds(0);
}
}  // namespace alive

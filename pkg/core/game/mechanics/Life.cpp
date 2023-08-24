#include "Life.hpp"

#include <SFML/System/Clock.hpp>

#include "GameField.hpp"
#include "GameObject.hpp"

namespace life {
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
}  // namespace life

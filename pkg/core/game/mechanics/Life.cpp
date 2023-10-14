#include "Life.hpp"

#include <SFML/System/Clock.hpp>

#include "GameField.hpp"
#include "GameObject.hpp"

namespace alive {
update timed(float seconds) {
    return [seconds, timer = sf::Clock()](const GameObject&, const GameField&) mutable {
        [[maybe_unused]] static auto _ = timer.restart();

        return timer.getElapsedTime().asSeconds() < seconds;
    };
}

update in_bounds(float margin) {
    return [margin](const GameObject& obj, const GameField& field) {
        return field.is_in_bounds(obj, margin);
    };
}
}  // namespace alive

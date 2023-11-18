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

update property_condition(const std::string& property, std::function<bool(float, float)> cond, float than) {
    return [property, cond, than](const GameObject& obj, const GameField&) {
        return cond(obj.props().get(property), than);
    };
}

update all(std::vector<update>&& updates) {
    return [updates = std::move(updates)](const GameObject& obj, const GameField& field) {
        for (auto& func : updates) {
            if (!func(obj, field)) {
                return false;
            }
        }
        return true;
    };
}

update any(std::vector<update>&& updates) {
    return [updates = std::move(updates)](const GameObject& obj, const GameField& field) {
        for (auto& func : updates) {
            if (func(obj, field)) {
                return true;
            }
        }
        return false;
    };
}
}  // namespace alive

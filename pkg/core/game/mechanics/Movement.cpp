#include "Movement.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameObject.hpp"
#include "LinAlg.hpp"

namespace {
bool keys_are_pressed(const KeyList& keys) {
    for (auto key : keys) {
        if (sf::Keyboard::isKeyPressed(key)) {
            return true;
        }
    }
    return false;
}

int horizontal(const KeyControls& keys, const JoyControls&) {
    if (keys_are_pressed(keys.left)) {
        return -1;
    }

    if (keys_are_pressed(keys.right)) {
        return 1;
    }

    return 0;
}

int vertical(const KeyControls& keys, const JoyControls&) {
    if (keys_are_pressed(keys.up)) {
        return -1;
    }

    if (keys_are_pressed(keys.down)) {
        return 1;
    }

    return 0;
}
}  // namespace

namespace movement {
std::unique_ptr<Func> linear(float x, float y) {
    sf::Vector2f velocity;
    return std::make_unique<Func>(Func::Type::Pos,
                                  [velocity = sf::Vector2f{x, y}](const GameObject& obj, float delta_time) {
                                      return obj.pos() + obj.speed() * velocity * delta_time;
                                  });
}

std::unique_ptr<Func> circular(sf::Vector2f radius, float speed) {
    return std::make_unique<Func>(
        Func::Type::Pos,
        [radius, speed, angle = atan(radius.y / radius.x), radius_len = abs(radius), started = false,
         center = sf::Vector2f{}](const GameObject& obj, float delta_time) mutable {
            if (!started) {
                center = radius + obj.pos();
                started = true;
            }

            angle += speed * delta_time;

            float x = center.x + radius_len * cos(angle);
            float y = center.y + radius_len * sin(angle);

            return sf::Vector2f{x, y};
        });
}

std::unique_ptr<Func> user_control(int user_num, const KeyControls& keys, const JoyControls& joy) {
    return std::make_unique<Func>(
        Func::Type::Velocity,
        [user_num, keys, joy](const GameObject&, float) {
            return sf::Vector2f{1.f * horizontal(keys, joy), 1.f * vertical(keys, joy)};
        },
        true);
}

Multi::Multi(std::vector<std::unique_ptr<Rule>>&& funcs, std::vector<float>&& times, Repeat repeat)
    : funcs_(std::move(funcs)), times_(std::move(times)), repeat_(repeat) {
    if (funcs_.size() > times_.size()) {
        throw std::runtime_error("you should supply time limit for each stage of movement rule");
    }
}

sf::Vector2f Multi::operator()(GameObject& obj, float delta) {
    if (current_time_ > times_[current_]) {
        ++current_;
        current_time_ = 0;
    }

    if (current_ >= funcs_.size()) {
        switch (repeat_) {
            case Repeat::Repeat:
                current_ = 0;
                break;
            case Repeat::Stop:
                return (*funcs_[current_ - 1])(obj, delta);
                break;
            case Repeat::StopAtLast:
                current_ = funcs_.size() - 1;
                break;
        }
    }

    current_time_ += delta;
    return (*funcs_[current_])(obj, delta);
}

Multi::operator bool() const {
    if (current_ >= funcs_.size()) {
        return false;
    }
    return bool(funcs_[current_]) && bool(*funcs_[current_]);
}

Rule::Type Multi::type() const {
    return funcs_[current_]->type();
}

bool Multi::moves_with_field() const {
    return funcs_[current_]->moves_with_field();
}

std::unique_ptr<Rule> Multi::clone() const {
    std::vector<std::unique_ptr<Rule>> funcs;
    funcs.reserve(funcs_.size());

    for (const auto& func : funcs_) {
        funcs.push_back(func->clone());
    }

    return std::make_unique<Multi>(std::move(funcs), std::vector(times_), repeat_);
}

void from_json(const nl::json& json, MultiInfo& info) {
    [[maybe_unused]] std::string contents = json.dump(4);
    if (json.contains("rules")) {
        json.at("rules").get_to(info.rules);
        json.at("repeat").get_to(info.repeat);
    } else {
        info.rules.push_back(json.template get<TimedFuncInfo>());
        json::get<Repeat>(json, "repeat", Repeat::Repeat);
    }
}

void to_json(nl::json& json, const MultiInfo& info) {
    if (info.rules.size() > 1) {
        json["rules"] = info.rules;
    } else {
        json = info.rules.front();
    }
    json["repeat"] = info.repeat;
}
}  // namespace movement

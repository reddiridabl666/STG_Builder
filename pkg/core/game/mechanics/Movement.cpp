#include "Movement.hpp"

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameObject.hpp"
#include "LinAlg.hpp"

namespace movement {
namespace {
bool keys_are_pressed(const KeyList& keys) {
    for (auto key : keys) {
        if (sf::Keyboard::isKeyPressed(key)) {
            return true;
        }
    }
    return false;
}

int horizontal(const MovementKeys& keys, const JoyControls&) {
    if (keys_are_pressed(keys.left)) {
        return -1;
    }

    if (keys_are_pressed(keys.right)) {
        return 1;
    }

    return 0;
}

int vertical(const MovementKeys& keys, const JoyControls&) {
    if (keys_are_pressed(keys.up)) {
        return -1;
    }

    if (keys_are_pressed(keys.down)) {
        return 1;
    }

    return 0;
}

struct Circular : public Rule {
  public:
    Circular(const sf::Vector2f& radius, float speed) : radius_(radius), speed_(speed), radius_len_(abs(radius)) {}

    Result operator()(const GameObject& obj, float delta_time) override {
        angle_ += obj.speed() * speed_ * delta_time;

        float x = center_.x + radius_len_ * cos(angle_);
        float y = center_.y + radius_len_ * sin(angle_);

        return Result{
            .type = Rule::Type::Pos,
            .pos = sf::Vector2f{x, y},
        };
    }

    bool moves_with_field() const override {
        return false;
    }

    operator bool() const override {
        return true;
    }

    void init(const GameObject& obj) override {
        center_ = radius_ + obj.pos();
        angle_ = atan(radius_.y / radius_.x);
    }

    std::unique_ptr<Rule> clone() const override {
        return std::make_unique<Circular>(radius_, speed_);
    }

  private:
    sf::Vector2f radius_;
    float speed_;

    float angle_;
    const float radius_len_;
    sf::Vector2f center_;
};
}  // namespace

std::unique_ptr<Rule> linear(float x, float y) {
    return std::make_unique<Func>([velocity = sf::Vector2f{x, y}](const GameObject&, float) {
        return Rule::Result{
            .type = Rule::Type::Velocity,
            .velocity = velocity,
        };
    });
}

std::unique_ptr<Rule> circular(const sf::Vector2f& radius, float speed) {
    return std::make_unique<Circular>(radius, speed);
}

std::unique_ptr<Rule> user_control(int user_num, const KeyControls& keys, const JoyControls& joy) {
    return std::make_unique<Func>(
        [user_num, keys, joy](const GameObject&, float) {
            return Rule::Result{
                .type = Rule::Type::Velocity,
                .velocity = sf::Vector2f{1.f * horizontal(keys.move, joy), 1.f * vertical(keys.move, joy)},
            };
        },
        true);
}

Multi::Multi(std::vector<std::unique_ptr<Rule>>&& funcs, std::vector<float>&& times, Repeat repeat)
    : funcs_(std::move(funcs)), times_(std::move(times)), repeat_(repeat) {
    if (funcs_.size() > times_.size()) {
        throw std::runtime_error("you should supply time limit for each stage of movement rule");
    }
}

Rule::Result Multi::operator()(const GameObject& obj, float delta) {
    if (current_ == funcs_.size() - 1 && repeat_ == Repeat::StopAtLast) {
        return (*funcs_[current_])(obj, delta);
    }

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
                return Result{
                    .type = Rule::Type::Pos,
                    .pos = obj.pos(),
                };
            case Repeat::StopAtLast:
                current_ = funcs_.size() - 1;
                return (*funcs_[current_])(obj, delta);
        }
    }

    if (abs(current_time_) < 1e-7) {
        funcs_[current_]->init(obj);
    }

    current_time_ += delta;
    return (*funcs_[current_])(obj, delta);
}

Multi::operator bool() const {
    if (current_ >= funcs_.size() && repeat_ != Repeat::StopAtLast) {
        return false;
    }
    return bool(funcs_[current_]) && bool(*funcs_[current_]);
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
    if (json.contains("rules")) {
        json.at("rules").get_to(info.rules);
        json.at("repeat").get_to(info.repeat);
    } else {
        info.rules.push_back(json.template get<TimedFuncInfo>());
        json.value("repeat", Repeat::Repeat);
    }
}

void to_json(nl::json& json, const MultiInfo& info) {
    if (info.rules.empty()) {
        return;
    }

    if (info.rules.size() > 1) {
        json["rules"] = info.rules;
    } else {
        json = info.rules.front();
    }
    json["repeat"] = info.repeat;
}
}  // namespace movement

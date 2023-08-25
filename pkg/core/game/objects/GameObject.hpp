#pragma once

#include <functional>
#include <memory>

#include "Errors.hpp"
#include "ImageContainer.hpp"
#include "Life.hpp"
#include "Movement.hpp"
#include "Properties.hpp"

class GameObject : public ImageContainer {
  public:
    static constexpr float kDefaultActivityStart = 0;
    static constexpr float kLoadDelta = 100;
    static const alive::update kDefaultLifeFunc;

    enum Tag {
        Object,
        Background,
        Item,
        Enemy,
        Bullet,
        Player
    };

    GameObject(const std::string& name, const sf::Vector2f& size, std::unique_ptr<Displayable>&& image,
               int speed = 50, Tag tag = Tag::Object, const Properties& props = {},
               float activity_start = kDefaultActivityStart,
               const alive::update& life_func = kDefaultLifeFunc,
               const movement::Func& move_func = movement::no_op, sf::Vector2f velocity = {},
               bool alive = true, bool active = false);

    GameObject(GameObject&& other);
    GameObject& operator=(GameObject&& other);

    float left() const;

    float right() const;

    float top() const;

    float bottom() const;

    float height() const;

    float width() const;

    void set_movement(const movement::Func& move) {
        move_update_ = move;
    }

    void set_movement(movement::Func&& move) {
        move_update_ = std::move(move);
    }

    // Hitbox& hitbox() {
    //     return hitbox_;
    // }

    Properties& props() {
        return props_;
    }

    void update(const GameField& field, float delta_time);

    Tag tag() const {
        return tag_;
    }

    void set_name(const std::string& name) {
        name_ = name;
    }

    const std::string& name() const {
        return name_;
    }

    const sf::Vector2f& velocity() const {
        return velocity_;
    }

    void set_velocity(const sf::Vector2f& velocity) {
        velocity_ = velocity;
    }

    bool is_alive() const {
        return alive_;
    }

    bool is_active() const {
        return active_;
    }

    void deactivate() {
        active_ = false;
    }

    void activate() {
        active_ = true;
    }

    void die() {
        alive_ = false;
    }

    void ressurect() {
        alive_ = true;
    }

    void set_activity(bool status) {
        active_ = status;
    }

    void set_activity_start(float activity_start) {
        activity_start_ = activity_start;
    }

    float activity_start() const {
        return activity_start_;
    }

    bool is_default_activatable() const {
        static constexpr float eps = 1e-2;
        return abs(activity_start_ - kDefaultActivityStart) < eps;
    }

    void set_life_update(const alive::update& func) {
        life_update_ = func;
    }

  private:
    std::string name_;
    Tag tag_;
    Properties props_;

    sf::Vector2f velocity_;
    movement::Func move_update_;
    alive::update life_update_ = kDefaultLifeFunc;

    bool active_ = false;
    bool alive_ = true;

    float activity_start_ = kDefaultActivityStart;

    void update_position(const GameField& field, float delta_time);
    bool update_activity(const GameField& field);
    void swap(GameObject& other);
};

inline std::ostream& operator<<(std::ostream& out, const GameObject& obj) {
    return out << obj.name();
}

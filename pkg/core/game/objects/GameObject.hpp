#pragma once

#include <functional>
#include <memory>

#include "Displayable.hpp"
#include "Errors.hpp"
#include "Movement.hpp"
#include "Properties.hpp"
#include "Updatable.hpp"

class GameObject : public Updatable, public Displayable {
  public:
    enum Tag {
        Object,
        Background,
        Item,
        Enemy,
        Bullet,
        Player
    };

    GameObject(std::unique_ptr<Displayable>&& image, int speed = 50, Tag tag = Tag::Object,
               const Properties& props = {})
        : tag_(tag), props_(props), speed_(speed), image_(std::move(image)) {}

    GameObject(const std::string& name, const sf::Vector2f& size, std::unique_ptr<Displayable>&& image,
               int speed = 50, Tag tag = Tag::Object, const Properties& props = {})
        : name_(name), tag_(tag), props_(props), speed_(speed), image_(std::move(image)) {
        set_size(size);
    }

    const std::unique_ptr<Displayable>& image() {
        return image_;
    }

    sf::Drawable& drawable() override {
        return image_->drawable();
    }

    sf::Transformable& transformable() override {
        return image_->transformable();
    }

    sf::Vector2f get_size() const override {
        return image_->get_size();
    }

    void set_movement(const movement::Func& move) {
        velocity_update_ = move;
    }

    void set_movement(movement::Func&& move) {
        velocity_update_ = std::move(move);
    }

    // Hitbox& hitbox() {
    //     return hitbox_;
    // }

    void set_speed(int speed) {
        speed_ = speed;
    }

    int speed() const {
        return speed_;
    }

    Properties& props() {
        return props_;
    }

    void update(float delta_time) override {
        if (velocity_update_) {
            velocity_ = velocity_update_(*this, delta_time);
        }
        move(speed_ * velocity_ * delta_time);
    }

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

  private:
    // Hitbox hitbox_;
    std::string name_;
    Tag tag_;
    Properties props_;

    sf::Vector2f velocity_;
    int speed_;

    std::unique_ptr<Displayable> image_;
    movement::Func velocity_update_;
};

inline std::ostream& operator<<(std::ostream& out, const GameObject& obj) {
    return out << obj.name();
}

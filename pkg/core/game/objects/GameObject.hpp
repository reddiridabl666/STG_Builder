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
    enum class Tag {
        Object,
        Background,
        Item,
        Enemy,
        EnemyBullet,
        Player
    };

    GameObject(std::unique_ptr<Displayable>&& image, int speed = 50, Tag tag = Tag::Object,
               const Properties& props = {})
        : tag_(tag), props_(props), speed_(speed), image_(std::move(image)) {}

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
        move_ = move;
    }

    void set_movement(movement::Func&& move) {
        move_ = std::move(move);
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
        move_(*this, delta_time);
    }

    Tag tag() const {
        return tag_;
    }

  private:
    // Hitbox hitbox_;
    Tag tag_;
    Properties props_;
    int speed_;
    std::unique_ptr<Displayable> image_;
    movement::Func move_;
};

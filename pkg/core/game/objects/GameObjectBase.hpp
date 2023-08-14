#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Updatable.hpp"

class GameObjectBase : public Updatable, public Displayable {
  public:
    GameObjectBase(std::unique_ptr<Displayable>&& image, int speed = 50)
        : speed_(speed), image_(std::move(image)) {}

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

    sf::FloatRect get_bounds() const override {
        return image_->get_bounds();
    }

    void set_speed(int speed) {
        speed_ = speed;
    }

    int speed() const {
        return speed_;
    }

  protected:
    int speed_ = 1;
    std::unique_ptr<Displayable> image_;
};

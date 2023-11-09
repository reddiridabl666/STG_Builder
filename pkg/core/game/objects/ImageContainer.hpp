#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Value.hpp"

class ImageContainer : public Displayable {
  public:
    ImageContainer(std::unique_ptr<Displayable>&& image, const Value& speed = 50)
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

    const Value& speed() const {
        return speed_;
    }

    Value& speed() {
        return speed_;
    }

  protected:
    Value speed_ = 50;
    std::unique_ptr<Displayable> image_;
};

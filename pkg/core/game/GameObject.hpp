#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Updatable.hpp"

class GameObject : public Updatable, public Displayable {
  public:
    GameObject(std::unique_ptr<Displayable>&& image) : image_(std::move(image)) {}

    const std::unique_ptr<Displayable>& image() {
        return image_;
    }

    sf::Drawable& drawable() override {
        return image_->drawable();
    }

    sf::Transformable& transformable() override {
        return image_->transformable();
    }

  private:
    std::unique_ptr<Displayable> image_;
};

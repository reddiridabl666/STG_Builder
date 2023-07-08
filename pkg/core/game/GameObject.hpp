#pragma once

#include <memory>

#include "Displayable.hpp"
#include "Observable.hpp"
#include "Updatable.hpp"

class GameObject : public Updatable, public Observable {
  public:
    GameObject(std::unique_ptr<Displayable>&& image) : image_(std::move(image)) {}

    const std::unique_ptr<Displayable>& get_image() {
        return image_;
    }

  private:
    std::unique_ptr<Displayable> image_;
};

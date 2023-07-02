#pragma once

#include "DrawableCollection.hpp"

class Window {
  public:
    [[nodiscard]] virtual bool is_open() const = 0;
    virtual void process_events() = 0;
    virtual void render(const DrawableCollection&) = 0;
    virtual void display() = 0;
};

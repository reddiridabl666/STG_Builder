#pragma once

#include "Collection.hpp"

class Drawable;
class UiElement;

class Window {
  public:
    [[nodiscard]] virtual bool is_open() const = 0;
    virtual void process_events() = 0;
    virtual void draw(const Drawable&) = 0;
    virtual void draw_ui(const Collection<UiElement>&) = 0;
    virtual void display() = 0;
    virtual ~Window() = default;
};

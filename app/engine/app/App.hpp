#pragma once

#include "Window.hpp"

class App {
  public:
    App(Window& window) : window_(window) {}
    bool run();

  private:
    Window& window_;
    Collection<Drawable> drawables_;
    Collection<UiElement> ui_elements_;
};

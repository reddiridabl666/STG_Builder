#pragma once

class UiElement {
  public:
    virtual void draw() const = 0;
    virtual ~UiElement() = default;
};

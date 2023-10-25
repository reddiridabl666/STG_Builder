#pragma once

#include <fmt/core.h>

#include "StatDisplay.hpp"
#include "Text.hpp"
#include "Value.hpp"

template <typename T = int>
class Counter : virtual public StatDisplay, public Text {
  public:
    Counter(Value<T>& val, std::shared_ptr<sf::Font>&& font, size_t size)
        : Text(fmt::format("{}", val_), std::move(font), size), val_(val) {}

    void update() {
        set_contents(fmt::format("{}", val_));  // perhaps this should be in an event handler
    }

  private:
    Value<T>& val_;
};

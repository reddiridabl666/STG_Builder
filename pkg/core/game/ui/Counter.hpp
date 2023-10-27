#pragma once

#include <fmt/core.h>

#include "GameUi.hpp"
#include "Text.hpp"
#include "Value.hpp"

class Counter : virtual public GameUi, public Text {
  public:
    Counter(float val, std::shared_ptr<sf::Font>&& font, size_t size) : Text("", std::move(font), size) {
        update(val);
    }

    void update(float val) override {
        set_contents(fmt::format("{}", val));
    }
};

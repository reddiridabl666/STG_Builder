#pragma once

#include <fmt/core.h>

#include "StatUi.hpp"
#include "Text.hpp"
#include "Value.hpp"

class Counter : public StatUi, public Text {
  public:
    Counter(float val, const std::string& prefix, std::shared_ptr<sf::Font>&& font, size_t size)
        : Text("", std::move(font), size), prefix_(prefix) {
        update(val);
    }

    void update(float val) override {
        set_contents(fmt::format("{} {}", prefix_, val));
    }

    void set_prefix(const std::string& prefix) {
        prefix_ = prefix;
    }

  protected:
    std::string prefix_;
};
